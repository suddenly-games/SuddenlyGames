#include "FrameBase.h"

extern "C" {
#include <math.h>
}

#include "Graphics.h"
#include "Textures.h"

const Mesh* FrameBase::ClippingMesh = nullptr;

FrameBase::FrameBase(FrameBase* parent)
{
	SetParent(parent);

	MouseEntered.Connect([this]() -> bool {
		MouseHovering = true;

		return true;
	});

	MouseLeft.Connect([this]() -> bool {
		MouseHovering = false;

		return true;
	});
}

FrameBase::~FrameBase()
{
	for (int i = 0; i < int(Children.size()); ++i)
		Children[i]->SetParent(nullptr);

	if (Parent != nullptr)
		SetParent(nullptr);
}

void FrameBase::Draw() const
{
	if (!Enabled)
		return;

	if (Visible)
		DrawFrame();

	for (int i = 0; i < ChildFrames.GetChildCount(); ++i)
		ChildFrames[i]->Draw();
}

void FrameBase::Update()
{
	if (Parent != nullptr)
	{
		AbsoluteSize = Size.Calculate(Vector3(), Parent->GetAbsoluteSize());
		LocalPosition = Position.Calculate(Vector3(), Parent->GetAbsoluteSize());
	}
	else
	{
		AbsoluteSize = Size.Calculate(Vector3(), Vector3());
		LocalPosition = Position.Calculate(Vector3(), Vector3());
	}

	if (LockAspectRatio)
		ClampSize(AbsoluteSize.X, AbsoluteSize.Y, AspectRatio);

	if (Parent != nullptr)
	{
		switch (AlignX)
		{
		case Enum::Alignment::Center:
			LocalPosition.X += 0.5f * (Parent->GetAbsoluteSize().X - AbsoluteSize.X);

			break;
		case Enum::Alignment::Maximum:
			LocalPosition.X += Parent->GetAbsoluteSize().X - AbsoluteSize.X;
		}

		switch (AlignY)
		{
		case Enum::Alignment::Center:
			LocalPosition.Y += 0.5f * (Parent->GetAbsoluteSize().Y - AbsoluteSize.Y);

			break;
		case Enum::Alignment::Maximum:
			LocalPosition.Y += Parent->GetAbsoluteSize().Y - AbsoluteSize.Y;
		}
	}

	Vector3 resolution = GetResolution();
	Vector3 scale(AbsoluteSize.X, AbsoluteSize.Y, 1);
	Vector3 parentSize = resolution;

	if (Parent != nullptr)
		parentSize = Parent->AbsoluteSize;

	Vector3 translation(2 * LocalPosition.X + AbsoluteSize.X - parentSize.X, parentSize.Y - AbsoluteSize.Y - 2 * LocalPosition.Y);
	Vector3 localPivot = PivotPoint.Calculate(Vector3(), AbsoluteSize);
	Vector3 pivot = Vector3(translation.X + 2 * localPivot.X - AbsoluteSize.X, translation.Y - 2 * localPivot.Y + AbsoluteSize.Y);

	Movement = Matrix3().Translate(pivot) * Matrix3().RotateAxis(Vector3(0, 0, 1), RotationAngle) * Matrix3().Translate(translation - pivot);

	if (Parent != nullptr)
		Movement = Parent->Movement * Movement;

	Transformation = Matrix3().Scale(1 / resolution.X, 1 / resolution.Y, 1) * Matrix3().Translate(Movement.Translation()) * Matrix3().ExtractRotation(Movement) * Matrix3().Scale(scale);
	InverseTransformation.Invert(Transformation);

	for (int i = 0; i < int(Children.size()); ++i)
		Children[i]->Update();

	//if (clipsDescendants)
	//	refreshClippingMask();

	UpdateFrame();
}

FrameBase* FrameBase::GetFocusFrame(const Vector3& point)
{
	if (!Enabled || !InputEnabled)
		return nullptr;

	bool contains = ContainsPoint(point);

	if (ClipsDescendants && !contains)
		return nullptr;

	for (int i = int(Children.size()) - 1; i >= 0; --i)
	{
		FrameBase* focus = Children[i]->GetFocusFrame(point);

		if (focus != nullptr)
			return focus;
	}

	if (Visible && contains)
		return InputRedirect != nullptr ? InputRedirect : this;

	return nullptr;
}

void FrameBase::SetParent(FrameBase* newParent)
{
	if (Parent == newParent)
		return;

	if (Parent != nullptr)
		Parent->RemoveChild(this);

	if (newParent != nullptr)
		newParent->AddChild(this);

	Parent = newParent;

	Update();
}

void FrameBase::SetSize(const DeviceVector& newSize)
{
	Size = newSize;

	Update();
}

void FrameBase::SetPosition(const DeviceVector& newPosition)
{
	Position = newPosition;

	Update();
}

void FrameBase::SetSizeAndPosition(const DeviceVector& newSize, const DeviceVector& newPosition)
{

	Size = newSize;
	Position = newPosition;

	Update();
}

void FrameBase::SetAspectRatio(float newAspectRatio)
{
	AspectRatio = newAspectRatio;
	LockAspectRatio = true;

	Update();
}

void FrameBase::SetAspectRatioLocked(bool mode)
{
	LockAspectRatio = mode;

	Update();
}

void FrameBase::SetAlignment(Alignment alignmentX, Alignment alignmentY)
{
	AlignX = alignmentX;
	AlignY = alignmentY;

	Update();
}

void FrameBase::SetAlignmentX(Alignment alignmentX)
{
	AlignX = alignmentX;

	Update();
}

void FrameBase::SetAlignmentY(Alignment alignmentY)
{
	AlignY = alignmentY;

	Update();
}

void FrameBase::SetRotation(float angle)
{
	RotationAngle = angle;

	Update();
}

void FrameBase::SetRotation(float angle, const DeviceVector& newPivotPoint)
{
	RotationAngle = angle;
	PivotPoint = newPivotPoint;

	Update();
}

void FrameBase::SetPivotPoint(const DeviceVector& newPivotPoint)
{
	PivotPoint = newPivotPoint;

	Update();
}

void FrameBase::SetClipsDescendants(bool clips)
{
	if (ClippingMesh == nullptr)
		return;

	ClipsDescendants = clips;

	Programs::ClippingMask->Use();
	Programs::ClippingMask->resolution.Set(GetResolution());

	RefreshClippingMask();
}

FrameBase* FrameBase::GetParent()
{
	return Parent;
}

const DeviceVector& FrameBase::GetSize() const
{
	return Size;
}

const DeviceVector& FrameBase::GetPosition() const
{
	return Position;
}

const Vector3& FrameBase::GetAbsoluteSize() const
{
	return AbsoluteSize;
}

Vector3 FrameBase::GetAbsolutePosition(const DeviceVector& point) const
{
	Vector3 localPoint(
		2 * point.X.Calculate(0, AbsoluteSize.X) / AbsoluteSize.X - 1,
		1 - 2 * point.Y.Calculate(0, AbsoluteSize.Y) / AbsoluteSize.Y
	);

	Vector3 globalPoint = Transformation * localPoint;
	Vector3 resolution = GetResolution();

	return Vector3(
		0.5f * (1 + globalPoint.X) * resolution.X,
		0.5f * (1 - globalPoint.Y) * resolution.Y
	);
}

Vector3 FrameBase::GetLocalPosition() const
{
	return LocalPosition;
}

Vector3 FrameBase::GetLocalPosition(const Vector3& point) const
{
	Vector3 resolution = GetResolution();

	Vector3 globalPoint(
		2 * point.X / resolution.X - 1,
		1 - 2 * point.Y / resolution.Y,
		0,
		1
	);

	Vector3 localPoint = InverseTransformation * globalPoint;

	return Vector3(
		0.5f * (localPoint.X + 1) * AbsoluteSize.X,
		-0.5f * (localPoint.Y - 1) * AbsoluteSize.Y
	);
}

const Matrix3& FrameBase::GetTransformation() const
{
	return Transformation;
}

const Matrix3& FrameBase::GetTransformationInverse() const
{
	return InverseTransformation;
}

float FrameBase::GetAspectRatio() const
{
	return AspectRatio;
}

bool FrameBase::AspectRatioLocked() const
{
	return LockAspectRatio;
}

Enum::Alignment FrameBase::GetAlignmentX() const
{
	return AlignX;
}

Enum::Alignment FrameBase::GetAlignmentY() const
{
	return AlignY;
}

float FrameBase::GetRotation() const
{
	return RotationAngle;
}

const DeviceVector& FrameBase::GetPivotPoint() const
{
	return PivotPoint;
}

Vector3 FrameBase::GetResolution() const
{
	const FrameBase* frame = this;

	while (frame->Parent)
		frame = frame->Parent;

	return frame->AbsoluteSize;
}

bool FrameBase::GetClipsDescendants() const
{
	return ClipsDescendants;
}

Handle<GraphicsEngine::Texture> FrameBase::GetClippingMask() const
{
	return ClippingMask;
}

bool FrameBase::ContainsPoint(const Vector3& point) const
{
	Vector3 localPoint = GetLocalPosition(point);
	
	return (
		localPoint.X >= 0 && localPoint.X <= AbsoluteSize.X &&
		localPoint.Y >= 0 && localPoint.Y <= AbsoluteSize.Y
	);
}

Matrix3 FrameBase::CalculateLocalTransformation(const Vector3& size, const Vector3& position) const
{
	Vector3 resolution = AbsoluteSize;
	Vector3 translation(
		(2 * position.X + size.X) / resolution.X - 1,
		1 - (2 * position.Y + size.Y) / resolution.Y
	);

	return Matrix3().Translate(translation) * Matrix3().Scale(size.X / resolution.X, size.Y / resolution.Y, 1);
}

Matrix3 FrameBase::CalculateScreenTransformation(const Vector3& size, const Vector3& position, const Vector3& resolution)
{
	Vector3 resolutionInverse(1 / resolution.X, 1 / resolution.Y, 1);
	Vector3 scale(size.X * resolutionInverse.X, size.Y * resolutionInverse.Y, 1);

	return Matrix3().Translate(2 * position.X * resolutionInverse.X + scale.X - 1, 1 - 2 * position.Y * resolutionInverse.Y - scale.Y, 0) * Matrix3().Scale(scale);
}

void FrameBase::ClampSize(float& x, float& y, float aspectRatio)
{
	float currentAspectRatio = x / y;

	if (currentAspectRatio > aspectRatio)
	{
		x *= aspectRatio / currentAspectRatio;
	}
	else if (currentAspectRatio < aspectRatio)
	{
		y *= currentAspectRatio / aspectRatio;
	}
}

int FrameBase::ChildWrapper::GetChildCount() const
{
	return int(Children.size());
}

FrameBase* FrameBase::ChildWrapper::operator[](int index)
{
	return Children[index];
}

const FrameBase* FrameBase::ChildWrapper::operator[](int index) const
{
	return Children[index];
}

void FrameBase::RemoveChild(const FrameBase* child)
{
	ChildVector::iterator i;

	for (i = Children.begin(); i != Children.end() && *i != child; ++i);

	if (i != Children.end())
		Children.erase(i);
}

void FrameBase::AddChild(FrameBase* child)
{
	Children.push_back(child);
}

void FrameBase::RefreshClippingMask()
{
	if (ClipsDescendants)
	{
		Vector3 resolution = GetResolution();

		if (ClippingBuffer != nullptr && ClippingBuffer->GetTexture()->GetWidth() != int(resolution.X) && ClippingBuffer->GetTexture()->GetHeight() != int(resolution.Y))
			ClippingBuffer.Erase();

		if (ClippingBuffer == nullptr)
			ClippingBuffer = GraphicsEngine::FrameBuffer::Create(int(resolution.X), int(resolution.Y), GraphicsEngine::Textures::Create((unsigned int)(resolution.X), (unsigned int)(resolution.Y)));

		ClippingBuffer->DrawTo();

		Graphics::SetClearColor(RGBA(0x00000000));
		Graphics::ClearScreen(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Programs::ClippingMask->transform.Set(Transformation);
		Programs::ClippingMask->hasParent.Set(Parent != nullptr && Parent->ClippingMask != nullptr);

		if (Parent != nullptr && Parent->ClippingMask != nullptr)
			Programs::ClippingMask->textureData.Set(Parent->ClippingMask);

		ClippingMesh->Draw();

		ClippingBuffer->Detatch();

		ClippingMask = ClippingBuffer->GetTexture();
	}
	else
	{
		if (Parent != nullptr)
			ClippingMask = Parent->ClippingMask;
		else
			ClippingMask.Erase();

		if (ClippingBuffer != nullptr)
			ClippingBuffer.Erase();
	}

	for (int i = 0; i < int(Children.size()); ++i)
		Children[i]->RefreshClippingMask();
}
