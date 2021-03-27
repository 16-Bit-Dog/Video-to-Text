//////////////////////
//Ariel G
//Exists to do some thing I needed to be done
//////////////////////
 
// CREDIT to Tapio Vierros and his amazing console API named rlutil

#pragma comment (lib, "windowscodecs.lib")

#include <windows.h>
  
#include <wincodec.h>
#include <iostream>
#include <vector>
#include <string>
#define WIN32_LEAN_AND_MEAN 

int GLOBALSCALE;
HANDLE hConsole;
CONSOLE_SCREEN_BUFFER_INFOEX info;
CONSOLE_FONT_INFOEX CFI;

//program requries AVX because I felt like it may do something - need to test
//requires CUDA enabled GPU - no idea what sm because still working on project

//this is a rasterizer of a image program
struct RGBA {

	int r;
	int g;
	int b;
	int a;


}colorBase;

struct RGB {
	int r;
	int g;
	int b;
}colorBase2;

std::vector<std::vector<RGBA>> texManage(std::vector < std::vector<RGBA>> vec);

IWICImagingFactory* imgFac;

class imageClassMain {

public:
//	std::vector<BYTE> byteBuffer;

	WICRect imgData;
	UINT pixw;
	UINT pixh;
	double resx;
	double resy;

	IWICBitmapDecoder* imgDecode;
	IWICBitmapSource* imgbitmap;

	std::string fileName;
	
	IWICBitmapFrameDecode* frame = NULL;
	
	WICPixelFormatGUID* pixform;

	std::vector<std::vector<RGBA>> byteBufferProper; //reformatted into width and height
	std::vector<std::vector<char>> charArr;
}img;

class ffm {

public:
	std::string fileName;
	std::string ffmpeg;
	imageClassMain img;


	//
	int imageIterator = 0;

	WICRect imgData;
	UINT pixw;
	UINT pixh;
	double resx;
	double resy;

	IWICBitmapDecoder* imgDecode;
	IWICBitmapSource* imgbitmap;

	IWICBitmapFrameDecode* frame = NULL;

	WICPixelFormatGUID* pixform;

	std::vector< std::vector<std::vector<RGB>> > byteBufferProper; //reformatted into width and height - extra vector to store new image
	std::vector< std::vector<std::vector<char>> > charArr;

}ffmpeg;


std::string LoadFile() { //run asyncronously to allow program to run while things run

	std::string fileNameS;

	std::cout << "\nput file path [drag and drop]: \n\n";

	std::cin >> fileNameS;
	std::string finalobj;
	for (auto i : fileNameS) {
		if (i != '"') {
			finalobj += i;
		}
	}
	return finalobj;
}

void LoadFfm() { //run asyncronously to allow program to run while things run

	std::string fileNameS;

	std::cout << "\nput ffmpeg path [drag and drop]: \n\n";

	std::cin >> fileNameS;

	for (auto i : fileNameS) {
		if (i != '"') {
			ffmpeg.ffmpeg += i;
		}
	}

}




std::vector<std::vector<char>> linearText(std::vector<std::vector<RGBA>> pDat) { //avx goes burr
	std::vector<std::vector<char>> charList(pDat.size());
	for (int i = 0; i < pDat.size(); i++) {
		charList[i].resize(pDat[0].size());
		for (int y = 0; y < pDat[0].size(); y++) {
			//switch() //I AM NOT USING SWITCH CASE SINCE I WILL MAKE MULTIPUL FILTERS AND WANT TO COPY PASTE THE BASE FUNCTION
			
			if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 700)
			charList[i][y] = '*';


			else if (pDat[i][y].r+ pDat[i][y].b + pDat[i][y].g > 600)
				charList[i][y] = '1';

			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 560) {
				charList[i][y] = 'O';
			}
			

			
			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 320) {
				charList[i][y] = 'o';
			}
			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 220) {
				charList[i][y] = '0';
			}
			
			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 120) {
				charList[i][y] = '|';
			}
			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 10) {
				charList[i][y] = '/';
			}
			else {

				charList[i][y] = '=';

			}
		
		}

	}


	return charList;
}

std::vector<std::vector<char>> linearTextRGB(std::vector<std::vector<RGB>> pDat) { //avx goes burr
	std::vector<std::vector<char>> charList(pDat.size());
	for (int i = 0; i < pDat.size(); i++) {
		charList[i].resize(pDat[0].size());
		for (int y = 0; y < pDat[0].size(); y++) {
			//switch() //I AM NOT USING SWITCH CASE SINCE I WILL MAKE MULTIPUL FILTERS AND WANT TO COPY PASTE THE BASE FUNCTION

			if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 700)
				charList[i][y] = '*';


			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 680)
				charList[i][y] = 'x';

			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 640) {
				charList[i][y] = 'z';
			}



			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 580) {
				charList[i][y] = 'u';
			}
			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 540) {
				charList[i][y] = 'l';
			}


			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 480) {
				charList[i][y] = 'o';
			}
			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 440) {
				charList[i][y] = 'v';
			}


			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 380) {
				charList[i][y] = 'a';
			}
			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 340) {
				charList[i][y] = '9';
			}


			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 280) {
				charList[i][y] = '8';
			}
			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 240) {
				charList[i][y] = '7';
			}

			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 180) {
				charList[i][y] = '6';
			}
			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 140) {
				charList[i][y] = '5';
			}

			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 80) {
				charList[i][y] = '4';
			}
			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 60) {
				charList[i][y] = '3';
			}

			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 30) {
				charList[i][y] = '2';
			}
			else if (pDat[i][y].r + pDat[i][y].b + pDat[i][y].g > 10) {
				charList[i][y] = '1';
			}
			else {

				charList[i][y] = '0';

			}

		}

	}


	return charList;
}


void printCharList(std::vector<std::vector<char>> tcl, std::vector<std::vector<RGBA>> rgb) { // made for 2000x2000 ish

	//get a divisor of x and y, since else we have too big! - not good for prime numbers

	int ratioy = tcl.size();
	int ratiox = tcl[0].size();

	int tmpyratio = 0;
	int tmpyratio2 = 0;

	int truey = 1;

	for (int i = 2; i < GLOBALSCALE; i++) { //optimised for 720p size - 1080p would work with 32 fairly well

		tmpyratio = ratioy % i;
		tmpyratio2 = ratiox % i;

		if (tmpyratio % 1 == 0 && tmpyratio2 % 1 == 0) { //decimal checker -- too big if not these ratios
			truey = i;
		}
	}

	ratioy = truey;
	
	ratiox = truey;

	CFI.cbSize = sizeof(CFI);
	CFI.nFont = 0;
	CFI.dwFontSize.X = 200 / (ceil(ratiox*1.5));                   // Width of each character in the font
	CFI.dwFontSize.Y = 200 / ((ratioy));                  // Height
	CFI.FontFamily = FF_DONTCARE;
	CFI.FontWeight = FW_NORMAL;
	std::wcscpy(CFI.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &CFI);

	info.srWindow = SMALL_RECT(0, 0, 720, 720);
	(SetConsoleScreenBufferInfoEx(hConsole, &info));


//	int XMS;
	for (int i = 0; i < tcl.size(); i += ratioy) {

		for (int z = 0; z < tcl[0].size(); z += ratiox) {

			printf("\x1B[48;5;%d;%d;%dm%c", rgb[i][z].r, rgb[i][z].g, rgb[i][z].b, tcl[i][z]);
		}
		printf("\x1B[40m\n"); //\x1b[1B
	}

}

void printCharListRGB(std::vector<std::vector<char>> tcl, std::vector<std::vector<RGB>> rgb) { // made for 2000x2000 ish

	//get a divisor of x and y, since else we have too big! - not good for prime numbers

	int ratioy = tcl.size();
	int ratiox = tcl[0].size();

	int tmpyratio = 0;
	int tmpyratio2 = 0;

	int truey = 1;

	for (int i = 2; i < GLOBALSCALE; i++) { //optimised for 720p size - 1080p would work with 32 fairly well

		tmpyratio = ratioy % i;
		tmpyratio2 = ratiox % i;

		if (tmpyratio % 1 == 0 && tmpyratio2 % 1 == 0) { //decimal checker -- too big if not these ratios
			truey = i;
		}
	}

	ratioy = truey;

	ratiox = truey;

	//
	CFI.cbSize = sizeof(CFI);
	CFI.nFont = 0;
	CFI.dwFontSize.X = 200 /(ceil(ratiox * 1.5));                   // Width of each character in the font
	CFI.dwFontSize.Y = 200 /((ratioy));                  // Height
	CFI.FontFamily = FF_DONTCARE;
	CFI.FontWeight = FW_NORMAL;
	std::wcscpy(CFI.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &CFI);


	info.srWindow = SMALL_RECT(0, 0, 720, 720);
	(SetConsoleScreenBufferInfoEx(hConsole, &info));


	//	int XMS;
	for (int i = 0; i < tcl.size(); i += ratioy) {

		for (int z = 0; z < tcl[0].size(); z += ratiox) {

			printf("\x1B[48;5;%d;%d;%dm%c", rgb[i][z].r, rgb[i][z].g, rgb[i][z].b, tcl[i][z]);
		}
		printf("\x1B[40m\n"); //\x1b[1B
	}

}
void NOTVID() {

	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory, (LPVOID*)&imgFac);
	if (FAILED(hr))
	{
		throw("cannot make image factory");
	}


	img.fileName = LoadFile();


	auto fileNameC = (std::wstring(img.fileName.begin(), img.fileName.end()));

	hr = imgFac->CreateDecoderFromFilename(LPCWSTR(fileNameC.c_str()), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &img.imgDecode);

	if (FAILED(hr)) {
		std::cout << GetLastError();
	}

	img.imgDecode->GetFrame(0, &img.frame);

	img.frame->GetSize(&img.pixw, &img.pixh);

	img.imgData.X = 0;
	img.imgData.Y = 0;
	img.imgData.Width = img.pixw;
	img.imgData.Height = img.pixh;


	img.frame->GetResolution(&img.resx, &img.resy);

	img.frame->GetPixelFormat(img.pixform);

	std::vector<BYTE> byteArray(img.pixh * ((img.pixw * 32 + 7) / 8)); // allocate max pos
	BYTE* byteVec = &byteArray[0];

	hr = img.frame->CopyPixels(&img.imgData, (img.pixw * 32 + 7) / 8, img.pixh * ((img.pixw * 32 + 7) / 8), byteVec); //assumes RBGA format... 32 bpp

	if (FAILED(hr)) {
		std::cout << GetLastError();
	}

	byteArray;

	std::vector<RGBA> byteBuffert;
	img.byteBufferProper.resize((byteArray.size()) / ((img.pixw * 32 + 7) / 8));

	for (int i = 0; i < img.byteBufferProper.size(); i++) {

		img.byteBufferProper[i].resize(img.pixw);

	}


	int tmp = -1;

	int precalc = (img.pixw * 32 + 7) / 8;

	int tmpiter = 0;

	for (int x = 0; x < byteArray.size(); x += 4) {
		colorBase.r = byteArray[x];
		colorBase.g = byteArray[x + 1];
		colorBase.b = byteArray[x + 2];
		colorBase.a = byteArray[x + 3];

		if ((x) % (precalc) == 0) {
			tmp += 1;
			tmpiter = 0;
		}
		img.byteBufferProper[tmp][tmpiter] = (colorBase);
		tmpiter += 1;
	}

	//volatile auto a = decltype(&img.byteBufferProper);

	//img.byteBufferProper = texManage(img.byteBufferProper);

	std::cout << "\n one last thing; enter the scale for the video - 10 is VERY BIG, 15 is large (good for a 15inch 1080p screen laptop), 20 is the smallest I would make to render detail on a colored video";
	std::cin >> GLOBALSCALE;

	img.charArr = linearText(img.byteBufferProper);

	printCharList(img.charArr, img.byteBufferProper);


}

void vid() {
	LoadFfm();
	std::cout << "\nenter video path now\n";
	ffmpeg.img.fileName = LoadFile();

	std::string VideoFileType = "";

	for (int i = ffmpeg.img.fileName.size()-1; i > 0; i--) {
		if (ffmpeg.img.fileName[i] == '.') {
			break;
		}
		else {
			VideoFileType = ffmpeg.img.fileName[i] + VideoFileType; //push back letter
		}
	}

	std::string OutputLoc;

	for (int i = ffmpeg.img.fileName.size() - 1; i > 0; i--) {
		if (ffmpeg.img.fileName[i] != '\\') {
			
		}
		else {
			
			OutputLoc = ffmpeg.img.fileName.substr(0,std::size_t(i)); //push back letter
			OutputLoc.append("\\%d.jpg");
			break;
		}
	}

	std::string command = ffmpeg.ffmpeg + " -r 1 -i " + ffmpeg.img.fileName + " -r 1 " + OutputLoc;

	system(command.c_str());

}

void loadManyImages() {
	int imgC = 0;
	std::cout << "\n\n\n" << "enter largest image (scroll to the bottom of the folder after sorted and enter the largest image number - which is the name\n\n";
	std::cin >> imgC;

	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
		IID_IWICImagingFactory, (LPVOID*)&imgFac);

	if (FAILED(hr))
	{
		throw("cannot make image factory");
	}
	std::string tmp;
	std::cout << "\ndrag and drop the folder that has images constructed with ffmpeg\n\n";
	std::cin >> ffmpeg.fileName;

	ffmpeg.fileName.append("\\");
	std::string fileNamePost;

	std::string fileBack = ".jpg";

	ffmpeg.byteBufferProper.resize(imgC);
	ffmpeg.charArr.resize(imgC);

	std::cout << "\n one last thing; enter the scale for the video - 10 is VERY BIG, 15 is large (good for a 15inch 1080p screen laptop), 20 is the smallest I would make to render detail on a colored video";
	std::cin >> GLOBALSCALE;

	std::cout << "processing.\n";
	for (int z = 0; z < imgC; z++) {
		std::cout << "Parsing image:" << z <<"\n";
		fileNamePost = ffmpeg.fileName + std::to_string(z+1) + fileBack;
		
		std::wstring strtemp = std::wstring(fileNamePost.begin(), fileNamePost.end());

		LPCWSTR fn = strtemp.c_str();
		
		hr = imgFac->CreateDecoderFromFilename(fn, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &ffmpeg.imgDecode);

		if (FAILED(hr)) {
			std::cout << GetLastError();
		}

		ffmpeg.imgDecode->GetFrame(0, &ffmpeg.frame);

		ffmpeg.frame->GetSize(&ffmpeg.pixw, &ffmpeg.pixh);

		ffmpeg.imgData.X = 0;
		ffmpeg.imgData.Y = 0;
		ffmpeg.imgData.Width = ffmpeg.pixw;
		ffmpeg.imgData.Height = ffmpeg.pixh;



		ffmpeg.frame->GetResolution(&ffmpeg.resx, &ffmpeg.resy);

		ffmpeg.frame->GetPixelFormat(ffmpeg.pixform);

		std::vector<BYTE> byteArray(ffmpeg.pixh * ((ffmpeg.pixw * 24 + 7) / 8)); // allocate max pos
		BYTE* byteVec = &byteArray[0];

		hr = ffmpeg.frame->CopyPixels(&ffmpeg.imgData, (ffmpeg.pixw * 24 + 7) / 8, ffmpeg.pixh * ((ffmpeg.pixw * 24 + 7) / 8), byteVec); //assumes RBG format... 24 bpp

		if (FAILED(hr)) {
			std::cout << GetLastError();
		}

		byteArray;

		std::vector<RGBA> byteBuffert;
		ffmpeg.byteBufferProper[z].resize((byteArray.size()) / ((ffmpeg.pixw * 24 + 7) / 8));

		for (int i = 0; i < ffmpeg.byteBufferProper[z].size(); i++) {

			ffmpeg.byteBufferProper[z][i].resize(ffmpeg.pixw);

		}


		int tmp = -1;

		int precalc = (ffmpeg.pixw * 24 + 7) / 8;

		int tmpiter = 0;

		for (int x = 0; x < byteArray.size(); x += 3) {
			colorBase2.r = byteArray[x];
			colorBase2.g = byteArray[x + 1];
			colorBase2.b = byteArray[x + 2];
			
			if ((x) % (precalc) == 0) {
				tmp += 1;
				tmpiter = 0;
			}
			ffmpeg.byteBufferProper[z][tmp][tmpiter] = (colorBase2);
			tmpiter += 1;
		}

		//volatile auto a = decltype(&img.byteBufferProper);

		//img.byteBufferProper = texManage(img.byteBufferProper);

		ffmpeg.charArr[z] = linearTextRGB(ffmpeg.byteBufferProper[z]);

	}
	for (int z = 0; z < imgC; z++) {
		
		printCharListRGB(ffmpeg.charArr[z], ffmpeg.byteBufferProper[z]);

		printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	}
}

int main() {
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD thing;
	GetConsoleMode(hConsole, &thing);
	
	SetConsoleMode(hConsole, thing | ENABLE_VIRTUAL_TERMINAL_PROCESSING | ENABLE_VIRTUAL_TERMINAL_INPUT);
	info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);//ENABLE_VIRTUAL_TERMINAL_INPUT

	info.dwSize = COORD(1, 0);

	

	GetConsoleScreenBufferInfoEx(hConsole, &info);


	std::string a;
	std::cout << "enter m for video worker; p for many images (already used ffmpeg)\n";
	std::cin >> a;
	
	if (a == "m" || a == "M") {

		//place holder
		vid();
	}
	else if (a == "p" || a == "P") {

		loadManyImages();

	}
	else {
		NOTVID();
	}
}
