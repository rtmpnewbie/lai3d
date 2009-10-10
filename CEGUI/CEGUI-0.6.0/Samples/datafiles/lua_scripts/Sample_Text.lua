print("Sample_Text begin")
local guiSystem = CEGUI.System:getSingleton()
local schemeMgr = CEGUI.SchemeManager:getSingleton()
local winMgr = CEGUI.WindowManager:getSingleton();
local fontMgr = CEGUI.FontManager:getSingleton();

-- load scheme and set up defaults
schemeMgr:loadScheme("TaharezLook.scheme");
guiSystem:setDefaultMouseCursor("TaharezLook", "MouseArrow");
-- We need a font
if(false == fontMgr:isFontPresent("DejaVuSans-10")) then
	fontMgr:createFont("DejaVuSans-10.font");
end
-- Font defaulting
if(fontMgr:isFontPresent("DejaVuSans-10")) then
	guiSystem:setDefaultFont("DejaVuSans-10");
elseif(fontMgr:isFontPresent("Commonwealth-10")) then
	guiSystem:setDefaultFont("Commonwealth-10");
end

-- load an image to use as a background
CEGUI.ImagesetManager:getSingleton():createImagesetFromImageFile("BackgroundImage", "GPN-2000-001437.tga");
-- here we will use a StaticImage as the root, then we can use it to place a background image
local background = winMgr:createWindow("TaharezLook/StaticImage", "background_wnd");
-- set position and size
local xx = CEGUI.UDim(0,0)
local yy = CEGUI.UDim(0,0)
local zz = CEGUI.UVector2(xx,yy)
background:setPosition(zz);

background:setSize(CEGUI.UVector2(CEGUI.UDim(1,0), CEGUI.UDim(1,0)));
-- disable frame and standard background
background:setProperty("FrameEnabled", "false");
background:setProperty("BackgroundEnabled", "false");
-- set the background image
background:setProperty("Image", "set:BackgroundImage image:full_image");
-- install this as the root GUI sheet
guiSystem:setGUISheet(background);

-- Load our layout as a basic
background:addChildWindow (winMgr:loadWindowLayout ("TextDemo.layout"));
	
print("Sample_Text OK")
