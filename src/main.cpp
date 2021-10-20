#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "shlwapi.h"
#include "Windows.h"
#include <regex>
#include <io.h>
#include <iomanip>
#include <ctime>
#define MINI_CASE_SENSITIVE
#include "ini.h"

#pragma comment(lib,"shlwapi.lib")
using namespace std;
string APP_PATH;

bool DirExists(string path)
{
	struct stat info;
	int statRC = stat(path.c_str(), &info);
	if (statRC != 0)
	{
		return false;
	}
	return (info.st_mode & S_IFDIR) ? true : false;
}

bool FileExists(string fileName)
{
	ifstream infile(fileName);
	return infile.good();
}

vector<string> explode(const string iDelimiter, const string iSubject)
{
	vector<string> arr;

	size_t strLeng = iSubject.length();
	size_t delLeng = iDelimiter.length();
	if (delLeng == 0)
		return arr;

	size_t i = 0;
	size_t j = 0;
	size_t k = 0;
	while (i < strLeng)
	{
		j = 0;
		while (static_cast<__int64>(i) + j < strLeng && j < delLeng && iSubject[(static_cast<__int64>(i) + j)] == iDelimiter[j])
			j++;
		if (j == delLeng)
		{
			arr.push_back(iSubject.substr(k, (static_cast<__int64>(i) - k)));
			i += delLeng;
			k = i;
		}
		else
		{
			i++;
		}
	}
	arr.push_back(iSubject.substr(k, (static_cast<__int64>(i) - k)));
	return arr;
}

void fCopy(string iSrc, string iDst) {
	ifstream  src(iSrc, ios::binary);
	ofstream  dst(iDst, ios::binary);
	dst << src.rdbuf();
}

string str_replace(const string search, const string replace, string subject)
{
	string buffer;

	size_t sealeng = search.length();
	size_t strleng = subject.length();

	if (sealeng == 0)
		return subject;

	for (size_t i = 0, j = 0; i < strleng; j = 0)
	{
		while (static_cast<__int64>(i) + j < strleng && j < sealeng && subject[static_cast<__int64>(i) + j] == search[j])
			j++;
		if (j == sealeng)
		{
			buffer.append(replace);
			i += sealeng;
		}
		else
		{
			buffer.append(&subject[i++], 1);
		}
	}
	subject = buffer;
	return subject;
}

string trim(const string& str,
	const string& whitespace = " \t")
{
	const auto strBegin = str.find_first_not_of(whitespace);
	if (strBegin == string::npos)
		return "";

	const auto strEnd = str.find_last_not_of(whitespace);
	const auto strRange = strEnd - strBegin + 1;

	return str.substr(strBegin, strRange);
}

void writeFile(string iPath, string iText, bool append = false) {
	ofstream MyFile;
	if (append == true) {
		MyFile.open(iPath, ios_base::app);
	}
	else {
		MyFile.open(iPath);
	}
	MyFile << iText.c_str();
	MyFile.close();
}

string readFile(string iPath, string iDelimiter = "\n") {
	string iContent;
	string iOutput;
	ifstream MyFile(iPath);

	int i = 0;
	while (getline(MyFile, iContent)) {
		if (i == 0) {
			iOutput = iContent;
		}
		else {
			iOutput += iDelimiter + iContent;
		}
		i++;
	}

	MyFile.close();

	return iOutput;
}
void startClean(int start = 0) {
	switch (start) {
	case 0:
		if (DirExists(APP_PATH + "_OUT")) {
			if (!DirExists(APP_PATH + "_OUT.bak")) {
				system(("mkdir " + APP_PATH + "_OUT.bak > NUL").c_str());
			}
			system(("move " + APP_PATH + "_OUT\\* " + APP_PATH + "_OUT.bak > NUL").c_str());
			system(("rmdir /s /q " + APP_PATH + "_OUT > NUL").c_str());
		}

		if (DirExists(APP_PATH + "workdir")) {
			system(("rmdir /s /q " + APP_PATH + "workdir > NUL").c_str());
		}
		else {
			system(("mkdir " + APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents > NUL").c_str());
			fCopy(APP_PATH + "assets\\template.xml ", APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\template.xml");
		}

		break;
	case 1:
		if (!DirExists(APP_PATH + "_OUT")) {
			system(("mkdir " + APP_PATH + "_OUT > NUL").c_str());
		}		
		if (DirExists(APP_PATH + "workdir")) {
			system(("rmdir /s /q " + APP_PATH + "workdir > NUL").c_str());
		}
		system(("mkdir " + APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents > NUL").c_str());
		fCopy(APP_PATH + "assets\\template.xml ", APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\template.xml");
		break;
	case 2:
		if (DirExists(APP_PATH + "workdir")) {
			system(("rmdir /s /q " + APP_PATH + "workdir > NUL").c_str());
		}
		break;
	}	
}

string strtoupper(string iSubject) {
	for_each(iSubject.begin(), iSubject.end(), [](char& c) {	c = ::toupper(c); });
	return iSubject;
}

string strtolower(string iSubject) {
	for_each(iSubject.begin(), iSubject.end(), [](char& c) {	c = ::tolower(c); });
	return iSubject;
}

string getFileExtension(string fPath, bool withDot = false) {
	if (withDot) {
		return PathFindExtensionA(fPath.c_str());
	}
	else {
		return str_replace(".", "", PathFindExtensionA(fPath.c_str()));
	}
}

string searchImage(string gamePath, string gameFilename, string imgType, string ext) {

	string imgLocationJPEG;
	string imgLocationPNG;
	string searchImage = "none";

	imgLocationJPEG = str_replace("." + ext, "_" + imgType + ".jpg", APP_PATH + "_IN\\" + str_replace("ux0:", "", gamePath));
	imgLocationPNG = str_replace("." + ext, "_" + imgType + ".png", APP_PATH + "_IN\\" + str_replace("ux0:", "", gamePath));
	if (FileExists(imgLocationJPEG) == true) {
		searchImage = imgLocationJPEG;
	}
	else {
		if (FileExists(imgLocationPNG) == true) {
			searchImage = imgLocationPNG;
		}
	}
	if (searchImage == "none") {
		if (FileExists(APP_PATH + "assets\\artwork\\" + ext + "\\" + imgType + ".png")) {
			searchImage = APP_PATH + "assets\\artwork\\" + ext + "\\" + imgType + ".png";
		}
		else {
			searchImage = APP_PATH + "assets\\artwork\\all\\" + imgType + ".png";
		}
	}
	return searchImage;
}

string SearchReplaceRegex(string inputStr, string SearchPattern, string ReplacePattern = "") {
	return regex_replace(inputStr, regex(SearchPattern), ReplacePattern);
}

string INIReadKey(string sSection, string sKey, string sINIFileName) {
	mINI::INIFile file(sINIFileName);
	mINI::INIStructure ini;
	file.read(ini);
	return ini[sSection][sKey].c_str();
}

string ROMPathCRC32(vector<string> arr, size_t start) {
	return arr.at(start).c_str();
}

class ClassROMInfo {
	public:
	string ID;
	string Filename;
	string Filename2;
	string Extension;
	string Title;
	string Path;
	string Path2;
	string Core;
	string ImageBG;
	string ImageStartup;
	string ImageIcon0;
};


int main(int argc, char* argv[]) {
	ClassROMInfo romInfo;
	int tmpInt;
	string thisEXE = argv[0];
	APP_PATH = str_replace(thisEXE.substr(thisEXE.find_last_of("/\\") + 1), "", argv[0]);
	startClean();

	cout << "Retro 2 Vita Bubble ROM converter v0.8" << endl << endl;
	cout << "Are you new with this tool? Start by reading the readme.txt first." << endl << endl;

	int mnuChoice;
	do {

		cout << "What would you like to do?" << endl << endl;
		cout << "[1] Convert ROM(s) to VPK using the existing ROM(s) on the PS Vita" << endl;
		cout << "[2] Convert ROMS to VPK while including them in the VPK" << endl;
		cout << "[3] Convert ROMS but compress them first (7z) to VPK while including them in the VPK" << endl;
		cout << "[4] Show <readme.txt> | Press [ENTER] to scroll down" << endl;
		cout << "[5] Show <readme.txt> using notepad (external program)" << endl;
		cout << "[9] Quit" << endl;
		cout << "Give me your choice please (1,2,3,4,5 or 9) : ";
		cin >> mnuChoice;

		if (mnuChoice == 4) {
			cout << endl << endl;
			system(("more " + APP_PATH + "readme.txt").c_str());
			cout << endl << endl;
		}

		if (mnuChoice == 5) {
			cout << endl << endl;
			cout << "In order to continue, please close notepad first.";
			system(("notepad " + APP_PATH + "readme.txt").c_str());
			system("cls");
		}

		if (mnuChoice == 9) {
			startClean(2);
			return 0;
		}

	} while (mnuChoice != 1 && mnuChoice != 2 && mnuChoice != 3);

	cout << "Scanning _IN folder for ROMS...";
	system((APP_PATH + "bin\\crc32.exe " + APP_PATH + "_IN\\* /D/F/R | findstr -iv .png | findstr -iv .jpg > " + APP_PATH + "workdir\\romcrc32.txt").c_str());

	if (readFile(APP_PATH + "workdir\\romcrc32.txt") != "") {
		int i = 0;
		int j = 0;

		string lineContent;
		string collectOutput;
		ifstream MyFile(APP_PATH + "workdir\\romcrc32.txt");
		while (getline(MyFile, lineContent)) {
			vector<string> arrROMCRC = explode(" - CRC32: ", lineContent);

			for (auto value : arrROMCRC) {
				if (i == 0 && j == 0) {
					collectOutput = value;
				}
				else {
					collectOutput += "del!mit" + value;
				}
				i++;
			}
			j++;
			i = 0;
			arrROMCRC.clear();

		}
		j = 0;

		vector<string> arrROMCRC = explode("del!mit", collectOutput);
		MyFile.close();
		cout << "Found " + to_string(arrROMCRC.size() / 2) + " ROMS" << endl << endl << "Start converting now..." << endl;

		string currentROM;
		string currentCRC;

		for (size_t i = 0; i < arrROMCRC.size(); i = i + 2) {
			clock_t start, end;
			start = clock();

			startClean(1);

			currentROM = ROMPathCRC32(arrROMCRC, i);
			currentCRC = ROMPathCRC32(arrROMCRC, i + 1);

			romInfo.ID = "R" + currentCRC;
			romInfo.Extension = strtolower(getFileExtension(currentROM));
			romInfo.Filename = currentROM.substr(currentROM.find_last_of("/\\") + 1);
			romInfo.Filename2 = str_replace("." + romInfo.Extension, "", romInfo.Filename);			

			if (INIReadKey("renameFile2GameTitle", romInfo.Filename, APP_PATH + "settings.ini") != "") {
				romInfo.Title = INIReadKey("renameFile2GameTitle", romInfo.Filename, APP_PATH + "settings.ini");
			}
			else {
				romInfo.Title = str_replace("] ", "]", romInfo.Filename2);
				romInfo.Title = str_replace(" [", "[", romInfo.Title);
				romInfo.Title = str_replace(" ~ ", "", romInfo.Title);
				romInfo.Title = str_replace("~ ", "", romInfo.Title);
				romInfo.Title = str_replace(" ~", "", romInfo.Title);
				romInfo.Title = str_replace("~", "", romInfo.Title);
				romInfo.Title = str_replace(") ", ")", romInfo.Title);
				romInfo.Title = str_replace(" (", "(", romInfo.Title);
				romInfo.Title = str_replace("} ", "}", romInfo.Title);
				romInfo.Title = str_replace(" {", "{", romInfo.Title);
				romInfo.Title = regex_replace(romInfo.Title, regex(R"(\([^()]*\))"), "");
				romInfo.Title = regex_replace(romInfo.Title, regex(R"(\[[^()]*\])"), "");
				romInfo.Title = regex_replace(romInfo.Title, regex(R"(\{[^()]*\})"), "");
				romInfo.Title = str_replace("  ", " ", romInfo.Title);
				romInfo.Title = str_replace("  ", " ", romInfo.Title);
				romInfo.Title = trim(romInfo.Title);
				if (str_replace(", The", "", romInfo.Title) != romInfo.Title) {
					romInfo.Title = "The " + str_replace(", The", "", romInfo.Title);
				}
			}

			if (romInfo.Title.length() > 50) {
				romInfo.Title = romInfo.Title.substr(0, 50);
			}

			romInfo.Core = INIReadKey("extToCore", romInfo.Extension, APP_PATH + "settings.ini");

			if (romInfo.Core == "") {
				continue;
			}
			if (mnuChoice == 2 || mnuChoice == 3) {
				if (INIReadKey("BIOS", romInfo.Extension, APP_PATH + "settings.ini") != "") {
					if (FileExists(APP_PATH + "assets\\BIOS\\" + INIReadKey("BIOS", romInfo.Extension, APP_PATH + "settings.ini"))) {
						fCopy(APP_PATH + "assets\\BIOS\\" + INIReadKey("BIOS", romInfo.Extension, APP_PATH + "settings.ini"), APP_PATH + "workdir\\package\\" + INIReadKey("BIOS", romInfo.Extension, APP_PATH + "settings.ini"));
					}
					else {
						cout << "ERROR !! - Skip ROM <" + romInfo.Filename + ">: BIOS file missing <\\assets\\BIOS\\" + INIReadKey("BIOS", romInfo.Extension, APP_PATH + "settings.ini") + ">" << endl;
						continue;
					}
				}
			}
			if (mnuChoice == 1) {
				romInfo.Path = "ux0:" + str_replace("\\", "/", str_replace(APP_PATH + "_IN\\", "", currentROM));
							}
			else if(mnuChoice == 2) {
				romInfo.Path = "ux0:app/" + strtoupper(romInfo.ID) + "/" + romInfo.Filename;
				fCopy(currentROM, APP_PATH + "workdir\\package\\" + romInfo.Filename);
			}
			else if (mnuChoice == 3) {
				romInfo.Path = "ux0:app/" + strtoupper(romInfo.ID) + "/" + romInfo.Filename;

				if (INIReadKey("doNotArchive", romInfo.Extension, APP_PATH + "settings.ini") == "") {
					romInfo.Path = str_replace(romInfo.Extension, "7z", romInfo.Path);
					system((APP_PATH + "bin\\7z.exe -mx" + INIReadKey("general", "compressionROM", APP_PATH + "settings.ini") + " a \"" + APP_PATH + "workdir\\package\\" + str_replace(romInfo.Extension, "7z", romInfo.Filename) + "\" \"" + currentROM + "\" > NUL").c_str());
				}
				else {
					fCopy(currentROM, APP_PATH + "workdir\\package\\" + romInfo.Filename);
				}
				
			}

			if (romInfo.Core == "n64") {
				fCopy(APP_PATH + "assets\\eboots\\eboot_N64.bin", APP_PATH + "workdir\\package\\eboot.bin");
				writeFile(APP_PATH + "workdir\\package\\args.txt", romInfo.Path);
			}
			else {
				fCopy(APP_PATH + "assets\\eboots\\eboot_RA.bin", APP_PATH + "workdir\\package\\eboot.bin");
				writeFile(APP_PATH + "workdir\\package\\core.txt", romInfo.Core);
				writeFile(APP_PATH + "workdir\\package\\rom.txt", romInfo.Path);
			}			
			system((APP_PATH +"bin\\vita-mksfoex.exe -s TITLE_ID=" + romInfo.ID + " \"" + romInfo.Title + "\" \"" + APP_PATH + "workdir\\package\\sce_sys\\param.sfo\"").c_str());
			fCopy(searchImage(str_replace(APP_PATH + "_IN\\", "", currentROM), romInfo.Filename, "icon0", strtolower(romInfo.Extension)), APP_PATH + "workdir\\package\\sce_sys\\icon0-fs8.png");
			fCopy(searchImage(str_replace(APP_PATH + "_IN\\", "", currentROM), romInfo.Filename, "bg", strtolower(romInfo.Extension)), APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\bg-fs8.png");
			fCopy(searchImage(str_replace(APP_PATH + "_IN\\", "", currentROM), romInfo.Filename, "startup", strtolower(romInfo.Extension)), APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\startup-fs8.png");
			system((APP_PATH + "bin\\scale.bat -source \"" + APP_PATH + "workdir\\package\\sce_sys\\icon0-fs8.png\" -target \"" + APP_PATH + "workdir\\package\\sce_sys\\icon0.png\" -max-height 128 -max-width 128 -keep-ratio no -force yes").c_str());
			system((APP_PATH + "bin\\scale.bat -source \"" + APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\bg-fs8.png\" -target \"" + APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\bg.png\" -max-height 540 -max-width 960 -keep-ratio no -force yes").c_str());
			system((APP_PATH + "bin\\scale.bat -source \"" + APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\startup-fs8.png\" -target \"" + APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\startup.png\" -max-height 158 -max-width 280 -keep-ratio no -force yes").c_str());
			remove((APP_PATH + "workdir\\package\\sce_sys\\icon0-fs8.png").c_str());
			remove((APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\bg-fs8.png").c_str());
			remove((APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\startup-fs8.png").c_str());
			system((APP_PATH + "bin\\pngquant.exe --speed 10 --force 256 \"" + APP_PATH + "workdir\\package\\sce_sys\\icon0.png").c_str());
			system((APP_PATH + "bin\\pngquant.exe --speed 10 --force 256 \"" + APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\bg.png").c_str());
			system((APP_PATH + "bin\\pngquant.exe --speed 10 --force 256 \"" + APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\startup.png").c_str());
			remove((APP_PATH + "workdir\\package\\sce_sys\\icon0.png").c_str());
			remove((APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\bg.png").c_str());
			remove((APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\startup.png").c_str());
			tmpInt = rename((APP_PATH + "workdir\\package\\sce_sys\\icon0-fs8.png").c_str(), (APP_PATH + "workdir\\package\\sce_sys\\icon0.png").c_str());
			tmpInt = rename((APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\bg-fs8.png").c_str(), (APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\bg.png").c_str());
			tmpInt = rename((APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\startup-fs8.png").c_str(), (APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\startup.png").c_str());
			system((APP_PATH + "bin\\7z.exe -mx" + INIReadKey("general", "compressionVPK", APP_PATH + "settings.ini") + " a \"" + APP_PATH + "_OUT\\" + strtoupper(romInfo.Extension) + "-" + romInfo.Title + "-" + romInfo.ID + ".vpk\" \"" + APP_PATH + "workdir\\package\\*\" > NUL").c_str());
			end = clock();
			cout << "Conversion done in " << setprecision(2) << fixed << (static_cast<double>(end) - static_cast<double>(start)) / CLOCKS_PER_SEC << "s: " + romInfo.Title + "." + romInfo.Extension << endl;
		}
		cout << endl << "Conversion job(s) finished, now just install the VPK files using Vitashell. Enjoy your bubbles, cheers!" << endl << endl;
		system(("explorer.exe \"" + APP_PATH + "_OUT\"").c_str());
	}
	else {
		cout << endl << endl << "There are no ROMS found inside the _IN folder, nothing to convert.";
	}
	startClean(2);
	cout << endl << endl << "Press any key to close this application...";
	system("pause > NUL");
	return 0;
}