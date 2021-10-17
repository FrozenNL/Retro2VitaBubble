The first fully automated RetroArch/Daedalus 2 PS Vita Bubble application, I tried to make it as easy as possible.

If your ROMS on your PS Vita are located at: ux0:ROMS/NES, ux0:ROMS/SNES, ux0:ROMS/GB+GBC and so on then just make sure
this have the same structure in _IN,  like  \_IN\ROMS\NES \_IN\ROMS\SNES \_IN\ROMS\GB+GBC

You can connect your PS Vita to your Windows PC using VitaShell, and drag your ROMS folder over to the _IN folder

You can also choose to pack the ROM inside the VPK (method 2), then the structure is not as important, just drop them
somewhere in _IN and you will be fine.

You can add custom images for:

icon0		Bubble Icon
bg		Background
startup		Startup Emblem

First it will try to load from your actual ROM folder, for example: if your ROM file is located in 
_IN\ROMS\SNES\HomeBrew (Unl).smc it will try to load:

     _IN\ROMS\SNES\HomeBrew (Unl)_icon0.jpg
     _IN\ROMS\SNES\HomeBrew (Unl)_icon0.png

     _IN\ROMS\SNES\HomeBrew (Unl)_bg.jpg
     _IN\ROMS\SNES\HomeBrew (Unl)_bg.png

     _IN\ROMS\SNES\HomeBrew (Unl)_startup.jpg
     _IN\ROMS\SNES\HomeBrew (Unl)_startup.png

If those images cannot be found, it will fall back to default images located at the app folder \assets\artwork\ it will
try the extension folder first (gb, nes, smc etc.) if it cannot find them, it falls back to \assets\artwork\all\ folder

Custom images will automatic resize and convert to 256 color PNG the files the PS Vita will accept, so no worries here
you are free to choose jpg or png extension.

There are two methods of creating the VPK:

Method 1: Link the VPK to your existing ROM already present on the VITA. Press 1 in the APP to use this method.
Method 2: Pack the ROM inside of your VPK. Press 2 in the APP to use this method and read the important message below

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!<IMPORTANT>!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

If you use method 2 (pack the ROM inside the VPK) then you have to unhide the app folder on your VITA, you can achieve
this by starting Vitashell, connect the Vita to the PC using USB and press <select> on the VITA, now press WIN+R at the
same time on your keyboard (run) and type cmd, if the SD2Vita connected to E: for example, you type in the black window

attrib -S -H E:\app

and that's really it, now RetroArch can see the app folder and make it possible to play directly from the packed ROMS

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!<IMPORTANT>!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

How to assign another core to an filetype: edit settings.ini and change settings as you like, it's flexible to even add
more systems which are supported by RetroArch if you like, after adding extension to settings.ini it will automatically
scan the _IN folder for the new filetype.

This program is created with Microsoft Visual Studio 2019 C++, you might or might not need some additional runtime libs
https://aka.ms/vs/16/release/vc_redist.x64.exe https://aka.ms/vs/16/release/vc_redist.x86.exe

For now it's Windows platform only but it should not be to hard to port it to a MAC/Linux version as well, if you have
done it yourself, I am willing to include it in the project and give you credits for it, I myself have no intentions of
creating it, I spend enough time on this project already.

Any questions? Email me at jordy-at-polfliet[d-o-t]net (bot protection) I might or might not respond fully, it depends
on the question and how many time I have at the moment.
