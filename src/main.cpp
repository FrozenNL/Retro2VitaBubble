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

std::string trim(const std::string& str,
	const std::string& whitespace = " \t")
{
	const auto strBegin = str.find_first_not_of(whitespace);
	if (strBegin == std::string::npos)
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
	string tmpStr;
	switch (start) {
	case 0: //setup all the folders ready for use; empty _OUT folder also
		tmpStr = "CMD /c rd /s /q " + APP_PATH + "_OUT > NUL";
		system(tmpStr.c_str());
		tmpStr = "CMD /c mkdir " + APP_PATH + "_OUT > NUL";
		system(tmpStr.c_str());
		tmpStr = "CMD /c rd /s /q " + APP_PATH + "workdir > NUL";
		system(tmpStr.c_str());
		tmpStr = "CMD /c mkdir " + APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents > NUL";
		system(tmpStr.c_str());
		fCopy(APP_PATH + "assets\\template.xml ", APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\template.xml");
		break;
	case 1: //just clean the workdir and make ready for use
		tmpStr = "CMD /c rd /s /q " + APP_PATH + "workdir > NUL";
		system(tmpStr.c_str());
		tmpStr = "CMD /c mkdir " + APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents > NUL";
		system(tmpStr.c_str());
		fCopy(APP_PATH + "assets\\template.xml ", APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\template.xml");
		break;
	case 2: //on exit this will be runned, it will delete the workdir but leave the _OUT
		tmpStr = "CMD /c rd /s /q " + APP_PATH + "workdir > NUL";
		system(tmpStr.c_str());
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

bool FileExists(string fileName)
{
	ifstream infile(fileName);
	return infile.good();
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

string INIReadSection(string sSection, string sINIFileName) {
	int i = 0;
	string sReturn;
	mINI::INIFile file(sINIFileName);
	mINI::INIStructure ini;
	file.read(ini);
	for (auto const& it : ini)
	{
		auto const& section = it.first;
		auto const& collection = it.second;
		for (auto const& it2 : collection)
		{
			if (section == sSection) {
				auto const& key = it2.first;
				auto const& value = it2.second;
				i++;
				if (i == 1) {
					sReturn = key;
				}
				else {
					sReturn += "del!mit" + key;
				}
			}
		}
	}
	return sReturn;
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

int main(int argc, char* argv[]) {
	
	string tmpStr;
	string tmpStr2;
	int tmpInt;
	string thisEXE = argv[0];
	APP_PATH = str_replace(thisEXE.substr(thisEXE.find_last_of("/\\") + 1), "", argv[0]);
	startClean(0);
	system("cls");
	vector<string> extToCore = explode("del!mit", INIReadSection("extToCore", APP_PATH + "settings.ini"));



	cout << "Retro 2 Vita Bubble ROM converter v0.7.\n\n";
	cout << "New with this tool ? It's the best to read the readme.txt first then...";

	int mnuChoice;
	do {

		cout << "\n\nWhat would you like to do?\n\n[1] Convert ROM(s) to VPK using the existing ROM(s) on the PS Vita.\n[2] Convert ROMS to VPK while including them in the VPK\n[3] Show <readme.txt>\n[4] Show <readme.txt> using notepad (external program)\n[9] Quit\n\nGive me your choice please (1, 2, 3, 4 or 9) : ";
		cin >> mnuChoice;

		if (mnuChoice == 3) {
			cout << "\n\n\n======================================================<readme.txt>====================================================\n\n\n";
			tmpStr = "more " + APP_PATH + "readme.txt";
			system(tmpStr.c_str());
			cout << "\n\n";
		}

		if (mnuChoice == 4) {
			cout << "\n\nIn order to continue, please close notepad first.";
			tmpStr = "notepad " + APP_PATH + "readme.txt";
			system(tmpStr.c_str());
			system("cls");
		}

		if (mnuChoice == 9) {
			startClean(2);
			return 0;
		}

	} while (mnuChoice != 1 && mnuChoice != 2);

	cout << "Scanning _IN folder for ROMS...";
	


	tmpStr = APP_PATH + "bin\\crc32.exe " + APP_PATH + "_IN\\* /D/F/R | findstr -iv .png | findstr -iv .jpg > " + APP_PATH + "workdir\\romcrc32.txt";
	
	system(tmpStr.c_str());

	if (readFile(APP_PATH + "workdir\\romcrc32.txt") != "") {
		int i = 0;
		int j = 0;
		string gameID;
		string gameFilename;
		string gameFilename2;
		string gameExtension;
		string gameTitle;
		string gamePath;
		string gameCore;
		string gameImageBG;
		string gameImageStartup;
		string gameImageIcon0;

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
		cout << "Found " + to_string(arrROMCRC.size() / 2) + " ROMS\n\nStart converting now...\n";

		string currentROM;
		string currentCRC;

		for (size_t i = 0; i < arrROMCRC.size(); i = i + 2) {
			clock_t start, end;
			start = clock();

			startClean(1);

			currentROM = ROMPathCRC32(arrROMCRC, i);
			currentCRC = ROMPathCRC32(arrROMCRC, i + 1);

			gameID = "R" + currentCRC;
			gameExtension = strtolower(getFileExtension(currentROM));
			gameFilename = currentROM.substr(currentROM.find_last_of("/\\") + 1);
			gameFilename2 = str_replace("." + gameExtension, "", gameFilename);

			

			if (INIReadKey("renameFile2GameTitle", gameFilename, APP_PATH + "settings.ini") != "") {
				gameTitle = INIReadKey("renameFile2GameTitle", gameFilename, APP_PATH + "settings.ini");
			}
			else {
				gameTitle = str_replace("] ", "]", gameFilename2);
				gameTitle = str_replace(" [", "[", gameTitle);
				gameTitle = str_replace(" ~ ", "", gameTitle);
				gameTitle = str_replace("~ ", "", gameTitle);
				gameTitle = str_replace(" ~", "", gameTitle);
				gameTitle = str_replace("~", "", gameTitle);
				gameTitle = str_replace(") ", ")", gameTitle);
				gameTitle = str_replace(" (", "(", gameTitle);
				gameTitle = str_replace("} ", "}", gameTitle);
				gameTitle = str_replace(" {", "{", gameTitle);
				gameTitle = regex_replace(gameTitle, regex(R"(\([^()]*\))"), "");
				gameTitle = regex_replace(gameTitle, regex(R"(\[[^()]*\])"), "");
				gameTitle = regex_replace(gameTitle, regex(R"(\{[^()]*\})"), "");
				gameTitle = str_replace("  ", " ", gameTitle);
				gameTitle = str_replace("  ", " ", gameTitle);
				gameTitle = trim(gameTitle);
				if (str_replace(", The", "", gameTitle) != gameTitle) {
					gameTitle = "The " + str_replace(", The", "", gameTitle);
				}
			}

			if (gameTitle.length() > 50) {
				gameTitle = gameTitle.substr(0, 50);
			}

			gameCore = INIReadKey("extToCore", gameExtension, APP_PATH + "settings.ini");

			if (gameCore == "") {
				continue;
			}
			if (mnuChoice == 2 && INIReadKey("BIOS", gameExtension, APP_PATH + "settings.ini") != "") {
				if (FileExists(APP_PATH + "assets\\BIOS\\" + INIReadKey("BIOS", gameExtension, APP_PATH + "settings.ini"))) {
					fCopy(APP_PATH + "assets\\BIOS\\" + INIReadKey("BIOS", gameExtension, APP_PATH + "settings.ini"), APP_PATH + "workdir\\package\\" + INIReadKey("BIOS", gameExtension, APP_PATH + "settings.ini"));
				}
				else {
					cout << "Skip ROM <" + gameFilename + ">: BIOS file missing <\\assets\\BIOS\\" + INIReadKey("BIOS", gameExtension, APP_PATH + "settings.ini") + ">\n";
					Sleep(1000);
					continue;
				}

			}

			if (mnuChoice == 2) {
				gamePath = "ux0:app/" + strtoupper(gameID) + "/" + gameFilename;
				fCopy(currentROM, APP_PATH + "workdir\\package\\" + gameFilename);
			}
			else {
				cout << "\n\n" + currentROM + "\n\n";
				gamePath = "ux0:" + str_replace("\\","/",str_replace(APP_PATH + "_IN\\", "", currentROM));
			}

			if (gameCore == "n64") {
				fCopy(APP_PATH + "assets\\eboots\\eboot_N64.bin", APP_PATH + "workdir\\package\\eboot.bin");
				writeFile(APP_PATH + "workdir\\package\\args.txt", gamePath);
			}
			else {
				fCopy(APP_PATH + "assets\\eboots\\eboot_RA.bin", APP_PATH + "workdir\\package\\eboot.bin");
				writeFile(APP_PATH + "workdir\\package\\core.txt", gameCore);
				writeFile(APP_PATH + "workdir\\package\\rom.txt", gamePath);
			}

			tmpStr = APP_PATH + "bin\\vita-mksfoex.exe -s TITLE_ID=" + gameID + " \"" + gameTitle + "\" \"" + APP_PATH + "workdir\\package\\sce_sys\\param.sfo\"";
			system(tmpStr.c_str());

			fCopy(searchImage(str_replace(APP_PATH + "_IN\\", "", currentROM), gameFilename, "icon0", strtolower(getFileExtension(gamePath))), APP_PATH + "workdir\\package\\sce_sys\\icon0-fs8.png");
			fCopy(searchImage(str_replace(APP_PATH + "_IN\\", "", currentROM), gameFilename, "bg", strtolower(getFileExtension(gamePath))), APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\bg-fs8.png");
			fCopy(searchImage(str_replace(APP_PATH + "_IN\\", "", currentROM), gameFilename, "startup", strtolower(getFileExtension(gamePath))), APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\startup-fs8.png");
			
			tmpStr = APP_PATH + "bin\\scale.bat -source \"" + APP_PATH + "workdir\\package\\sce_sys\\icon0-fs8.png\" -target \"" + APP_PATH + "workdir\\package\\sce_sys\\icon0.png\" -max-height 128 -max-width 128 -keep-ratio no -force yes";
			system(tmpStr.c_str());
			tmpStr = APP_PATH + "bin\\scale.bat -source \"" + APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\bg-fs8.png\" -target \"" + APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\bg.png\" -max-height 540 -max-width 960 -keep-ratio no -force yes";
			system(tmpStr.c_str());
			tmpStr = APP_PATH + "bin\\scale.bat -source \"" + APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\startup-fs8.png\" -target \"" + APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\startup.png\" -max-height 158 -max-width 280 -keep-ratio no -force yes";
			system(tmpStr.c_str());

			tmpStr = APP_PATH + "workdir\\package\\sce_sys\\icon0-fs8.png";
			remove(tmpStr.c_str());
			tmpStr = APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\bg-fs8.png";
			remove(tmpStr.c_str());
			tmpStr = APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\startup-fs8.png";
			remove(tmpStr.c_str());

			tmpStr = APP_PATH + "bin\\pngquant.exe --speed 10 --force 256 \"" + APP_PATH + "workdir\\package\\sce_sys\\icon0.png";
			system(tmpStr.c_str());
			tmpStr = APP_PATH + "bin\\pngquant.exe --speed 10 --force 256 \"" + APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\bg.png";
			system(tmpStr.c_str());
			tmpStr = APP_PATH + "bin\\pngquant.exe --speed 10 --force 256 \"" + APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\startup.png";
			system(tmpStr.c_str());

			tmpStr = APP_PATH + "workdir\\package\\sce_sys\\icon0.png";
			remove(tmpStr.c_str());
			tmpStr = APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\bg.png";
			remove(tmpStr.c_str());
			tmpStr = APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\startup.png";
			remove(tmpStr.c_str());

			tmpStr = APP_PATH + "workdir\\package\\sce_sys\\icon0-fs8.png";
			tmpStr2 = APP_PATH + "workdir\\package\\sce_sys\\icon0.png";
			tmpInt = rename(tmpStr.c_str(), tmpStr2.c_str());

			tmpStr = APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\bg-fs8.png";
			tmpStr2 = APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\bg.png";
			tmpInt = rename(tmpStr.c_str(), tmpStr2.c_str());

			tmpStr = APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\startup-fs8.png";
			tmpStr2 = APP_PATH + "workdir\\package\\sce_sys\\livearea\\contents\\startup.png";
			tmpInt = rename(tmpStr.c_str(), tmpStr2.c_str());

			tmpStr = APP_PATH + "bin\\7z.exe -mx"+ INIReadKey("general", "vpkCompression", APP_PATH + "settings.ini") +" a \"" + APP_PATH + "_OUT\\" + strtoupper(gameExtension) + "-" + gameTitle + "-" + gameID + ".vpk\" \"" + APP_PATH + "workdir\\package\\*\" > NUL";
			system(tmpStr.c_str());
			end = clock();

			
			cout << "Conversion done in " << setprecision(2) << fixed << (static_cast<double>(end) - static_cast<double>(start)) / CLOCKS_PER_SEC << "s: " + gameTitle + "." + gameExtension + "\n";

		}
		cout << "\nConversion job(s) finished, now just install the VPK files using Vitashell. Enjoy your bubbles, cheers!\n\n";
		tmpStr = "explorer.exe \"" + APP_PATH + "_OUT\"";
		system(tmpStr.c_str());
	}
	else {
		cout << "\n\nThere are no ROMS found inside the _IN folder, nothing to convert.";
	}
	startClean(2);
	cout << "\n\nPress any key to close this application...";
	system("pause > NUL");
	return 0;
}