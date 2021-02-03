local env = require("./game/environment")
local userInput = Engine.GameWindow.UserInput
local scene = env.NewScene()

local characters = {
  { 
    SPD = 100,
    ATB = 0
  },
  {
    SPD = 146,
    ATB = 0
  },
  {
    SPD = 173,
    ATB = 0
  },
  {
    SPD = 82,
    ATB = 0
  },
  {
    SPD = 97,
    ATB = 0
  },
  {
    SPD = 123,
    ATB = 0
  },
}

local turnQueue = {}

local DisplayActionBar = function(character, position)

  local actionBar = scene.CreateSprite("UIActionBar")

  if position > 3 then
    actionBar.AnchorPoint = DeviceVector(1,0,0,0)
    actionBar.Position = DeviceVector(0, 1900, 0, (position - 3) * 100)
  else
    actionBar.AnchorPoint = DeviceVector(0,0,0,0)
    actionBar.Position = DeviceVector(0, 20, 0, position * 100)
  end

  while true do
    actionBar.Size = DeviceVector(0, 5 + character.ATB/10000 * 300, 0, 20)
    wait()
  end

end

local Initialize = function()

  for position, character in ipairs(characters) do
    --coroutine.wrap(DisplayActionBar)(character, position)
  end

  hello = scene.CreateText("lynx tuft frogs\n dolphins abduct by proxy the ever awkward klutz\n dud\n dummkopf\n jinx snubnose filmgoer\n orphan sgt\n renfruw grudgek reyfus\n md\n sikh psych if halt tympany jewelry sri heh\n twyer vs jojo pneu fylfot alcaaba son of nonplussed halfbreed bubbly playboy guggenheim daddy coccyx sgraffito effect\n vacuum dirndle impossible attempt to disvalue\n muzzle the afghan czech czar and exninja\n Bob bixby dvorak wood dhurrie savvy\n dizzy eye aeon circumcision uvula scrungy picnic luxurious special type carbohydrate ovoid adzuki kumquat bomb\n afterglows gold girl pygmy gnome lb\n ankhs acme aggroupment akmed brouhha tv wt\n ujjain ms\n oz abacus mnemonics bhikku khaki bwana aorta embolism vivid owls often kvetch otherwise\n wysiwyg densfort wright you’ve absorbed rhythm\n put obstacle kyaks krieg kern wurst subject\n enmity equity coquet quorum pique tzetse hepzibah sulfhydryl briefcase ajax ehler kafka\n fjord elfship halfdressed jugful eggcup hummingbirds swingdevil bagpipe legwork\n reproachful hunchback archknave baghdad wejh rijswijk rajbansi\n rajput ajdir okay weekday obfuscate subpoena liebknecht marcgravia ecbolic\n arcticward dickcissel pincpinc boldface maidkin adjective adcraft adman dwarfness\n applejack darkbrown kiln palzy always farmland flimflam unbossy nonlineal\n stepbrother lapdog stopgap sx countdown basketball beaujolais vb\n flowchart aztec\n lazy bozo syrup tarzan annoying dyke yucky hawg gagzhukz cuzco squire when\n hiho mayhem nietzsche szasz gumdrop milk emplotment ambidextrously\n lacquer byway ecclesiastes stubchen hobgoblins crabmill aqua hawaii blvd\n subquality byzantine empire debt obvious cervantes\n jekabzeel anecdote flicflac mechanicville bedbug couldn’t i’ve it’s they’ll they’d dpt\n headquarter burkhardt xerxes atkins govt\n ebenezer lg\n lhama amtrak amway fixity axmen quumbabda upjohn hrumpf")
  hello.Canvas.Text.FontSize = DeviceAxis(0, 26)
  hello.Size = DeviceVector(0, 1920, 0, 1080)
end

local PlayerTurn = function(character)
  local input = userInput:GetInput(Enum.InputCode.Enter)
  while not input:GetState() do 
    wait()
  end
  wait(0.1)
end

local Update = function()
  
  for _, character in ipairs(characters) do
    character.ATB = character.ATB + character.SPD
    if character.ATB >= 10000 then
      table.insert(turnQueue, character)
    end
  end

  table.sort(turnQueue, function(a,b) return a.ATB > b.ATB end)

  for i, character in ipairs(turnQueue) do
    PlayerTurn(character)
    character.ATB = character.ATB - 10000
  end

  turnQueue = {}

  wait()
end

Initialize()

while true do
  Update()
end