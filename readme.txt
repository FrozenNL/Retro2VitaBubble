-----------------------------------------
/         version: v0.8                 /
\    last updated: october 20th 2021    \
/         revised: 1 time               /
-----------------------------------------

First of all, for this tool you need:

-An homebrew accepted aka jailbroken PS Vita.
-(if you want many ROMS but not N64) You need to have RetroArch for Vita https://www.retroarch.com/?page=platforms 
-(if you want N64) Daedalus X64 v0.6 or later https://vitadb.rinnegatamante.it/#/info/549

Features:

 - Create bubbles for the Vita for RetroArch and Daedalus X64
 - Possiblity to pack the ROM inside the VPK package, also with 7-ZIP support
 - Flexible and really easy system, easy to add more systems as long as Retroarch support it (edit settings.ini)
 - Fully automated processing of your ROMS 2 VPK
 - Possibility to make a custom bubble, background and startup emblem (not fully automated but easy to add them)
 - Made with passion and love <3

The first fully automated RetroArch/Daedalus X64 2 PS Vita Bubble app (ROM2VPK), I made it as smooth as possible.

At first I made it for my self, but there was pretty much time involving in making it, I decided to polish it some 
more and publish it for the public, I really hope you enjoy the project and the ease of it.

If your ROMS on your PS Vita are located at: ux0:ROMS/NES, ux0:ROMS/SNES, ux0:ROMS/GB+GBC and so on then make sure
this have the same structure in _IN,  like  \_IN\ROMS\NES \_IN\ROMS\SNES \_IN\ROMS\GB+GBC

You can connect your PS Vita to your Windows PC using VitaShell, and drag your ROMS folder over to the _IN folder

You can also choose to pack the ROM inside the VPK (method 2), then the structure is not as important, just drop the
ROMS somewhere in _IN and you will be fine.

Accepted extensions are: 32x a26 a52 a78 bat chd col cue dsk exe fds gb gba gbc gen gg iso j64 jag lnx md mx1 mx2 n64
nes ngc ngp pce sfc sg smc smd sms st sv v64 ws wsc z64 zip (zip=neogeo, not compressed ROMS).

For some extensions (iso, chd, cue) you need to alter the ini file since they are used by multiple systems, you can't
simply process two of those extensions at the same time, you have to alter the ini file and asign different cores for
it and add those systems 1 by 1 in the _IN folder, create VPK, move the VPK from the _OUT folder  otherwise RetroArch
do not know what to do with it since a wrong core is assigned to it. By default those extensions are disabled.

You can add custom images for:

icon0 =	Bubble Icon
bg =      Background
startup = Startup Emblem

At first it will try to load from your actual ROM folder, for example: if your ROM file is located in let's say
_IN\ROMS\SNES\HomeBrew (Unl).smc then it will try to load the images:

       _IN\ROMS\SNES\HomeBrew (Unl)_icon0.jpg or _IN\ROMS\SNES\HomeBrew (Unl)_icon0.png
          _IN\ROMS\SNES\HomeBrew (Unl)_bg.jpg or _IN\ROMS\SNES\HomeBrew (Unl)_bg.png
     _IN\ROMS\SNES\HomeBrew (Unl)_startup.jpg or _IN\ROMS\SNES\HomeBrew (Unl)_startup.png

If the above images cannot be found, it falls back to default images from the \assets\artwork folder it'll try the 
extension folder first (gb, nes, smc etc.) if it cannot find them, it falls back to \assets\artwork\all folder

Custom images will automatic resize and convert to 256 color PNG this are the files the PS Vita will accept, so no
worries here you are free to choose jpg or png extension.

There are two methods of creating the VPK:

Method 1: Link the VPK to your existing ROM already present on the VITA. Press 1 in the APP to use this method.
Method 2: Pack the ROM inside of your VPK. Press 2 or 3 in the APP to use this method and read the message below.

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!<IMPORTANT>!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

If you use method 2 (pack the ROM inside the VPK) then you have to unhide the app folder on your VITA, you can achieve
this by starting Vitashell, connect the Vita to the PC using USB and press <select> on the VITA, now press WIN+R at the
same time on your keyboard (run) and type cmd, if the SD2Vita connected to E: for example, you type in the black window

attrib -S -H E:\app   --------------------------   this makes sure it sets it's attributes to non-system and non-hidden

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

TODO:

- Hoping there will also be Amiga (uae4all2) support, there needs to be a starter for it in order to create this, I am
willing to build support for it once the starter eboot is available and once the aue4all2 supports to open files from
a parameter.

Any questions, or found any bugs? Please email me at jordy-at-polfliet[d-o-t]net (bot protection) I try to respond ASAP.
