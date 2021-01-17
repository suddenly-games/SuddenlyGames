#pragma once

#include <vector>

#include "Matrix3.h"
#include "DeviceVector.h"
#include "InputEvent.h"
#include "FrameBuffer.h"
#include "Mesh.h"
#include "DeviceTransform.h"

class FrameBase
{
private:
	typedef std::vector<FrameBase*> ChildVector;

	ChildVector Children;

public:
	typedef Enum::Alignment Alignment;

	MouseInput Mouse;
	Event<> MouseEntered;
	Event<> MouseLeft;
	bool MouseHovering = false;

	bool Visible = true;
	bool Enabled = true;
	bool InputEnabled = true;

	FrameBase* InputRedirect = nullptr;

	static const Mesh* ClippingMesh;

	FrameBase(FrameBase* parent = nullptr);
	~FrameBase();

	void Draw() const;
	virtual void DrawFrame() const {};
	virtual void UpdateFrame() {}

	void Update();
	FrameBase* GetFocusFrame(const Vector3& point);
	virtual void SetParent(FrameBase* newParent);
	void SetSize(const DeviceVector& newSize);
	void SetPosition(const DeviceVector& newPosition);
	void SetSizeAndPosition(const DeviceVector& newSize, const DeviceVector& newPosition);
	void SetAspectRatio(float newAspectRatio);
	void SetAspectRatioLocked(bool mode);
	void SetAlignment(Alignment alignmentX, Alignment alignmentY);
	void SetAlignmentX(Alignment alignmentX);
	void SetAlignmentY(Alignment alignmentY);
	void SetRotation(float angle);
	void SetRotation(float angle, const DeviceVector& newPivotPoint);
	void SetPivotPoint(const DeviceVector& newPivotPoint);
	void SetClipsDescendants(bool clips);
	FrameBase* GetParent();
	const DeviceVector& GetSize() const;
	const DeviceVector& GetPosition() const;
	const Vector3& GetAbsoluteSize() const;
	Vector3 GetAbsolutePosition(const DeviceVector& point = DeviceVector(0, 0, 0, 0)) const;
	Vector3 GetLocalPosition() const;
	Vector3 GetLocalPosition(const Vector3& point) const;
	const Matrix3& GetTransformation() const;
	const Matrix3& GetTransformationInverse() const;
	float GetAspectRatio() const;
	bool AspectRatioLocked() const;
	Alignment GetAlignmentX() const;
	Alignment GetAlignmentY() const;
	float GetRotation() const;
	const DeviceVector& GetPivotPoint() const;
	Vector3 GetResolution() const;
	bool GetClipsDescendants() const;
	std::shared_ptr<GraphicsEngine::Texture> GetClippingMask() const;
	bool ContainsPoint(const Vector3& point) const;
	Matrix3 CalculateLocalTransformation(const Vector3& size, const Vector3& position) const;

	static Matrix3 CalculateScreenTransformation(const Vector3& size, const Vector3& position, const Vector3& resolution);
	static void ClampSize(float& x, float& y, float aspectRatio);

	class ChildWrapper
	{
	public:
		ChildWrapper(ChildVector& children) : Children(children) {}

		int GetChildCount() const;
		FrameBase* operator[](int index);
		const FrameBase* operator[](int index) const;

	private:
		ChildVector& Children;
	};

	ChildWrapper ChildFrames = ChildWrapper(Children);
	void RefreshClippingMask();

private:
	bool Updated = false;
	bool ClippingUpdated = false;
	Alignment AlignX = Alignment::Minimum;
	Alignment AlignY = Alignment::Minimum;
	bool LockAspectRatio = false;
	float AspectRatio = 1;
	float RotationAngle = 0;
	FrameBase* Parent = nullptr;
	DeviceVector Size = DeviceVector(1, 0, 1, 0);
	DeviceVector Position = DeviceVector(0, 0, 0, 0);
	DeviceVector PivotPoint = DeviceVector(0.5f, 0, 0.5f, 0);
	Vector3 AbsoluteSize;
	Vector3 LocalPosition;
	Matrix3 Transformation;
	Matrix3 InverseTransformation;
	Matrix3 Movement;
	std::shared_ptr<GraphicsEngine::FrameBuffer> ClippingBuffer;
	std::shared_ptr<GraphicsEngine::Texture> ClippingMask;
	bool ClipsDescendants = false;

	void RemoveChild(const FrameBase* child);
	void AddChild(FrameBase* child);
};
