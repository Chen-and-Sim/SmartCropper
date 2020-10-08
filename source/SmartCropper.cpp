#include <cstring>
#include <cstdlib>
#include <fstream>
#include <io.h>
#include <iostream>
#include <vector>
#include "SmartCropper.h"

#pragma comment(lib, "libgdiplus.a")
#include <windows.h>
#include <gdiplus.h>
#include <shlobj.h>

using namespace std;
using namespace Gdiplus;

void about(char* str) 
{
	if(language == English)
	{
		cout << "智剪 SmartCropper v1.0.0913\n"
			  << "(c) 2020 Ji-woon Sim, Wenge Chen\n"
			  << "Feedback Email: rcswex@163.com\n"
			  << "Feedback QQ: 925792714\n\n";
	}
	else
	{
		cout << "智剪 SmartCropper v1.0.0913\n"
			  << "(c) 2020 沈智云，陈文戈\n"
			  << "问题反馈邮箱：rcswex@163.com\n"
			  << "问题反馈QQ：925792714\n\n";
	}
}

void help(char* str)
{
	if(language == English)
	{
		cout << "\nAvailable commands: (command1  command2  description)\n"
			  << "-a  about     Show the information of the program.\n"
			  << "-h  help      Show help.\n"
			  << "-l  language  Switch the language of the program.\n"
			  << "-q  quit      Quit the program.\n"
			  << "-s  save      Save the cropped pictures.\n"
			  << "-x  settings  Show current settings.\n\n"
			  << "-i [Path]             load [Path]               Load an image. [Path] can be either absolute or relative,\n"
				  "                                                including extension.\n"
			  << "-u [Path]             loadall [Path]            Load all images in the folder located at [Path]. [Path] can be\n"
				  "                                                either absolute or relative.\n"
			  << "-f [Format]           format [Format]           Choose the format of output file(s). [Format] can be 'JPG' or\n"
				  "                                                'PNG' in either upper or lower case.\n" 
			  << "-n [String]           name [String]             Set output filename to [String]. Available wildcards: %f - original\n"
				  "                                                filename; %s - index of selected area; %0s, %00s etc. - add '0'\n"
				  "                                                in front of the index; %s+x - begin numbering from x+1. [DOES NOT\n"
				  "                                                DO ANY INPUT CHECK.]\n"
			  << "-p [Path]             path [Path]               Set the path of output file(s) to [Path]. 'Desktop' in either\n"
				  "                                                upper or lower case is reserved for the shortcut to your desktop.\n"
				  "                                                [DOES NOT DO ANY INPUT CHECK.]\n\n"
			  << "-c [Begin] [End]      create [Begin] [End]      Create an area that contains blocks ranging from [Begin] to [End].\n"
			  << "-d [Number]           delete [Number]           Delete the selected area whose index is [Number].\n"
			  << "-e [Num1] [Num2]      separate [Num1] [Num2]    Separate the selected area [Num1] along the block [Num2]. The\n"
				  "                                                block [Num2] will belong to the former one of the separated areas.\n"
			  << "-r [Begin] [End]      merge [Begin] [End]       Merge the selected areas whose indexes lie within [Begin] to [End].\n"
			  << "-m [Beg] [End] [Num]  modify [Beg] [End] [Num]  Modify the selected area whose index is [Num]. The range of\n"
				  "                                                blocks in this area will be changed as from [Beg] to [End].\n"
			  << "-v [Number]           preview [Number]          Preview the selected area whose index is [Number].\n\n"
			  << "-b [Height] [Span]    setmerge [Height] [Span]  Set block height and span threshold for auto-merging.\n"
			  << "-t [Value]            tolerance [Value]         Change the tolerance value to [Value]. A coloured pixel is more\n"
				  "                                                likely to be regarded as background if the value is set higher.\n"
				  "                                                The range is 0 to 255.\n\n";
	}
	else
	{
		cout << "\n可用的指令：（指令1  指令2  说明）\n"
			  << "-a  about     显示程序信息。\n"
			  << "-h  help      显示帮助。\n"
			  << "-l  language  切换程序语言。\n"
			  << "-q  quit      结束程序。\n"
			  << "-s  save      保存截图。\n"
			  << "-x  settings  显示当前设置。\n\n"
			  << "-i [Path]             load [Path]               载入图片。[Path]项可以是图片的绝对路径或相对路径，须包含后缀名。\n"
			  << "-u [Path]             loadall [Path]            载入路径为[Path]的文件夹内的所有图片。[Path]项可以是图片的绝对路\n"
				  "                                                径或相对路径。\n"
			  << "-f [Format]           format [Format]           选择输出格式。[Format]项可为JPG或PNG（不区分大小写）。\n" 
			  << "-n [String]           name [String]             将输出文件名改为[String]。可用通配符：%f - 原文件名；%s - 选区编\n"
				  "                                                号；%0s, %00s 等 - 选区编号前补0；%s+x - 从x+1开始进行编号。【不\n"
				  "                                                进行输入检查。】\n"
			  << "-p [Path]             path [Path]               将输出文件的路径设为[Path]。输入Desktop可将路径设为桌面（不区分\n"
			  << "                                                大小写）。【不进行输入检查。】\n\n"
			  << "-c [Begin] [End]      create [Begin] [End]      创建由区块[Begin]到[End]组成的选区。\n"
			  << "-d [Number]           delete [Number]           删除编号为[Number]的选区。\n"
			  << "-e [Num1] [Num2]      separate [Num1] [Num2]    以编号为[Num2]的区块为分界线分割编号为[Num1]的选区。编号为[Num2]\n"
				  "                                                的区块将属于前半个选区。\n" 
			  << "-r [Begin] [End]      merge [Begin] [End]       合并编号在[Begin]到[End]之间的选区。\n"
			  << "-m [Beg] [End] [Num]  modify [Beg] [End] [Num]  修改编号为[Num]的选区。选区内区块的范围改为[Beg]到[End]。\n"
			  << "-v [Number]           preview [Number]          预览编号为[Number]的选区。\n\n"
			  << "-b [Height] [Span]    setmerge [Height] [Span]  将自动合并的高度上限和间隔上限分别设置为[Height]和[Span]。\n"
			  << "-t [Value]            tolerance [Value]         将容差值改变为[Value]。当容差值较大时，有色的像素更有可能被视为\n"
				  "                                                背景像素、取值范围是0到255。\n\n";
	}
}

void switch_language(char* str)
{
	if(language == English)
	{
		language = Chinese;
		cout << "语言已切换为中文。\n\n";
	}
	else
	{
		language = English;
		cout << "Language has been switched to English.\n\n";
	}
}

void quit(char* str)
{
	if(access("__temp__", X_OK) == 0)
		system("rmdir /s /q __temp__");
	exit(0);
}

void save(char* str)
{
	if(!check_loaded())  return;

	char cmd1[200] = "\0";
	strcat(cmd1, output_path);
   for(int i = 0; i < strlen(cmd1); ++i)
   {
   	if(cmd1[i] == '/')
   		cmd1[i] = '\\';
	}
	if(access(cmd1, X_OK) != 0)
	{
		char cmd2[200] = "mkdir ";
		strcat(cmd2, cmd1);
		system(cmd2);
	}

	for(int i = 0; i < sel_area_count; ++i)
	{
		char output_name[200];
		wchar_t w_output_name[200];
		set_output_name(output_name, i);
		mbstowcs(w_output_name, output_name, strlen(output_name));
		w_output_name[strlen(output_name)] = L'\0';
		
		CLSID encoderClsid;
	   if(output_format == PNG)
	   	GetEncoderClsid(L"image/png", &encoderClsid);
	   else
	   	GetEncoderClsid(L"image/jpeg", &encoderClsid);

		int begin = sel_area_begin[i];
		int end = sel_area_end[i];	
		int y_begin = block_Y_begin[begin];
		int y_end = block_Y_end[end];
		int x_begin = block_X_begin[begin];
		int x_end = block_X_end[begin];
		for(int j = begin; j <= end; ++j)
		{
			if(block_X_begin[j] < x_begin)
				x_begin = block_X_begin[j];
			if(block_X_end[j] > x_end)
				x_end = block_X_end[j];
		}
		Bitmap* cur_bitmap = new Bitmap(w_cur_name);
		Bitmap* output_bitmap = cur_bitmap -> Clone(x_begin, y_begin, x_end - x_begin + 1,
																  y_end - y_begin + 1, PixelFormat24bppRGB);
		output_bitmap -> Save(w_output_name, &encoderClsid, NULL);
		delete cur_bitmap, output_bitmap;
	}

	if(language == English)
		cout << "Saved successfully.\n\n";
	else
		cout << "保存成功。\n\n";
}

void current_settings(char* str)
{
	if(!check_loaded())  return;
	if(language == English)
	{
		cout << "\nLanguage: English\n"
			  << "Name of current file: " << cur_filename << endl
			  << "Format of current file: " << ((cur_format == PNG) ? "PNG\n" : "JPG\n")
			  << "Path of current file: " << cur_path << endl
			  << "Name of output file(s): " << output_name_format << endl
			  << "Format of output file(s): " << ((output_format == PNG) ? "PNG\n" : "JPG\n")
			  << "Path of output file(s): " << output_path << "\n\n"
			  << "Number of blocks: " << block_count << endl
			  << "Number of selected areas: " << sel_area_count << endl;
		for(int i = 0; i < sel_area_count; ++i)
			cout << "    Selected area #" << i + 1 << ": block " << sel_area_begin[i] + 1 << " - " << sel_area_end[i] + 1 << endl;
		cout << "\nTolerance value: " << tolerance << endl
			  << "Auto-merge height threshold: " << merge_height << endl
			  << "Auto-merge span threshold: " << merge_span << "\n\n";
	}
	else
	{
		cout << "\n语言：中文\n"
			  << "当前文件名称：" << cur_filename << endl
			  << "当前文件格式：" << ((cur_format == PNG) ? "PNG\n" : "JPG\n")
			  << "当前文件路径：" << cur_path << endl
			  << "输出文件名称：" << output_name_format << endl
			  << "输出文件格式：" << ((output_format == PNG) ? "PNG\n" : "JPG\n")
			  << "输出文件路径：" << output_path << "\n\n"
			  << "区块数量：" << block_count << endl
			  << "选区数量：" << sel_area_count << endl;
		for(int i = 0; i < sel_area_count; ++i)
			cout << "    选区 #" << i + 1 << "：区块" << sel_area_begin[i] + 1 << " - " << sel_area_end[i] + 1 << endl;
		cout << "\n容差值：" << tolerance << endl
			  << "自动合并高度上限：" << merge_height << endl
			  << "自动合并间隔上限：" << merge_span << "\n\n";
	}
}

void load_image(char* str)
{
	ifstream fin;
	fin.open(str);
	if(!fin || !check_format(str))
	{
		if(language == English)
			cout << "Failed to load image.\n\n";
		else
			cout << "加载图片失败。\n\n";
		fin.clear();
		fin.close();
		return;
	}
	
	fin.close();
	set_name(str);
	loaded = true;
	
	Bitmap* cur_bitmap = new Bitmap(w_cur_name);
	Color color;
	const int width = cur_bitmap -> GetWidth();
	const int height = cur_bitmap -> GetHeight();
	int val = 0;
	for(int y = 0; y < height; ++y)
	{
		cur_bitmap -> GetPixel(y * width / height, y, &color);
		val += (255 - min( min(color.GetR(), color.GetG()), color.GetB() ));
	}
	tolerance = val / height;
	delete cur_bitmap;
	
	if(language == English)
		cout << "Successfully loaded image.\n\n";
	else
		cout << "成功加载图片。\n\n";
	detect_block();
}

void load_all(char* str)
{
	if(access(str, X_OK) == -1)
	{
		if(language == English)
			cout << "The folder does not exist or it does not have read/write permission.\n\n";
		else
			cout << "文件夹不存在或无读写权限。\n\n";
		return;
	}
	
   struct _finddata_t fileinfo;
   char str1[200];
	strcpy(str1, str);
	strcat(str1, "\\*");
	intptr_t handle = _findfirst(str1, &fileinfo);
	vector<Bitmap*> bitmaps;
	int total_width = 0, total_height = 0;
	
	if(handle != -1)
	{
		do{
			if( (strcmp(fileinfo.name, ".") != 0) && (strcmp(fileinfo.name, "..") != 0)
				 && check_format(fileinfo.name) )
			{
				char str2[200];
				strcpy(str2, str);
				strcat(str2, "\\");
				strcat(str2, fileinfo.name);
				set_name(str2);
				Bitmap* cur_bitmap = new Bitmap(w_cur_name);
				bitmaps.push_back(cur_bitmap);
				const int width = cur_bitmap -> GetWidth();
				const int height = cur_bitmap -> GetHeight();
				if(width > total_width)
					total_width = width;
				total_height += height;
			}
   	}	while(_findnext(handle, &fileinfo) == 0);
	}
	_findclose(handle);

   if(bitmaps.size() == 0)
   {
   	if(language == English)
			cout << "Image not found.\n\n";
		else
			cout << "未找到图片文件。\n\n";
		return;
	}

	Bitmap* merged = new Bitmap(total_width, total_height, PixelFormat24bppRGB);
	Graphics graphics(merged);
	int _y = 0;
	for(int i = 0; i < bitmaps.size(); ++i)
	{
		const int width = bitmaps[i] -> GetWidth();
		const int height = bitmaps[i] -> GetHeight();
		int scaled_height = height * total_width / width;
		graphics.DrawImage(bitmaps[i], 0, _y, total_width, scaled_height);
		_y += scaled_height;
	}
	
	Color color;
	int val = 0;
	for(int y = 0; y < total_height; ++y)
	{
		merged -> GetPixel(y * total_width / total_height, y, &color);
		val += (255 - min( min(color.GetR(), color.GetG()), color.GetB() ));
	}
	tolerance = val / total_height;

	CLSID encoderClsid;
	GetEncoderClsid(L"image/png", &encoderClsid);
	if(access("__temp__", X_OK) != 0)
		system("mkdir __temp__");
	merged -> Save(L"__temp__\\merged.png", &encoderClsid, NULL);
	
	if(language == English)
		cout << "Successfully loaded " << bitmaps.size() << " image(s).\n\n";
	else
		cout << "成功加载" << bitmaps.size() << "张图片。\n\n";
	delete merged;
	for(int i = 0; i < bitmaps.size(); ++i)
		delete bitmaps[i];
	loaded = true;
	set_name("__temp__\\merged.png");
	cur_format = PNG;
	output_format = PNG;
	detect_block();
}

void set_output_format(char* str)
{
	for(int i = 0; i < strlen(str); ++i)
	{
		if(str[i] >= 'a' && str[i] <= 'z')
			str[i] -= 32;
	}
	
	if(strcmp(str, "PNG") == 0)
		output_format = PNG;
	else if(strcmp(str, "JPG") == 0)
		output_format = JPG;
	else
	{
		if(language == English)
			cout << "Wrong command. Please check your input and try again.\n\n";
		else
			cout << "错误的指令。请检查输入后重试。\n\n";
		return;
	}
	if(language == English)
		cout << "Output format has been set to " << str << ".\n\n";
	else
		cout << "输出格式已成功设为" << str << "。\n\n";
}

void set_name_format(char* str)
{
	strcpy(output_name_format, str);
	if(language == English)
		cout << "Success.\n\n";
	else
		cout << "设置成功。\n\n";
}

void set_output_path(char* str)
{
	char copy[200];
	strcpy(copy, str);
	for(int i = 0; i < strlen(copy); ++i)
	{
		if(copy[i] >= 'A' && copy[i] <= 'Z')
			copy[i] += 32;
	}
	if(strcmp(copy, "desktop") == 0)
	{
		LPITEMIDLIST pidl;
		LPMALLOC pShellMalloc;
		if(SUCCEEDED(SHGetMalloc(&pShellMalloc)))
   	{
   		if( SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &pidl)) )
			{
            SHGetPathFromIDListA(pidl, output_path);
            pShellMalloc -> Free(pidl);
      	}
			pShellMalloc -> Release();
		}
	}
	else
		strcpy(output_path, str);

	int len = strlen(output_path);
	if(len != 0 && output_path[len - 1] != '/' && output_path[len - 1] != '\\')
	{
		output_path[len] = '\\';
		output_path[len + 1] = '\0';
	}
	
	if(language == English)
		cout << "Success.\n\n";
	else
		cout << "设置成功。\n\n";
}

void create_area(char* str)
{
	if(!check_loaded())  return;
	
	char str1[10], str2[10];
	int pos = 0;
	while(pos < strlen(str) && str[pos] != ' ')
		++pos;
	strncpy(str1, str, pos);
	str1[pos] = '\0';
	while(pos < strlen(str) && str[pos] == ' ')
		++pos;
	strcpy(str2, str + pos);
	
	char* pEnd;
	int begin = strtol(str1, &pEnd, 10);
	int end = strtol(str2, &pEnd, 10);
	if(begin <= 0 || begin > end || end > block_count)
	{
		if(language == English)
			cout << "Invalid input. Please check your input and try again.\n\n";
		else
			cout << "输入错误。请检查输入后重试。\n\n";
		return;
	}
	
	++sel_area_count;
	sel_area_begin.push_back(begin - 1);
	sel_area_end.push_back(end - 1);
	if(language == English)
	{
		cout << "Succeeded to create.\n\n" << "Current selected areas:\n";
		for(int i = 0; i < sel_area_count; ++i)
			cout << "Selected area #" << i + 1 << ": block " << sel_area_begin[i] + 1 << " - " << sel_area_end[i] + 1 << endl;
	}
	else
	{
		cout << "创建成功。\n\n" << "当前选区：\n";
		for(int i = 0; i < sel_area_count; ++i)
			cout << "选区 #" << i + 1 << "：区块" << sel_area_begin[i] + 1 << " - " << sel_area_end[i] + 1 << endl;
	}
	cout << endl;
}

void delete_area(char* str)
{
	if(!check_loaded())  return;
	
	char* pEnd;
	int index = strtol(str, &pEnd, 10);
	if(index <= 0 || index > sel_area_count)
	{
		if(language == English)
			cout << "Invalid input. Please check your input and try again.\n\n";
		else
			cout << "输入错误。请检查输入后重试。\n\n";
		return;
	}
	
	--sel_area_count;
	sel_area_begin.erase(sel_area_begin.begin() + index - 1);
	sel_area_end.erase(sel_area_end.begin() + index - 1);
	if(language == English)
	{
		cout << "Succeeded to delete.\n\n" << "Current selected areas:\n";
		for(int i = 0; i < sel_area_count; ++i)
			cout << "Selected area #" << i + 1 << ": block " << sel_area_begin[i] + 1 << " - " << sel_area_end[i] + 1 << endl;
	}
	else
	{
		cout << "删除成功。\n\n" << "当前选区：\n";
		for(int i = 0; i < sel_area_count; ++i)
			cout << "选区 #" << i + 1 << "：区块" << sel_area_begin[i] + 1 << " - " << sel_area_end[i] + 1 << endl;
	}
	cout << endl;
}

void separate_area(char* str)
{
	if(!check_loaded())  return;
	
	char str1[10], str2[10];
	int pos = 0;
	while(pos < strlen(str) && str[pos] != ' ')
		++pos;
	strncpy(str1, str, pos);
	str1[pos] = '\0';
	while(pos < strlen(str) && str[pos] == ' ')
		++pos;
	strcpy(str2, str + pos);
	
	char* pEnd;
	int pos1 = strtol(str1, &pEnd, 10);
	int pos2 = strtol(str2, &pEnd, 10);
	if(pos1 <= 0 || pos1 > sel_area_count || pos2 <= sel_area_begin[pos1 - 1] || pos2 > sel_area_end[pos1 - 1])
	{
		if(language == English)
			cout << "Invalid input. Please check your input and try again.\n\n";
		else
			cout << "输入错误。请检查输入后重试。\n\n";
		return;
	}
	
	int temp = sel_area_end[pos1 - 1];
	sel_area_end[pos1 - 1] = pos2 - 1;
	sel_area_begin.insert(sel_area_begin.begin() + pos1, pos2);
	sel_area_end.insert(sel_area_end.begin() + pos1, temp);
	++sel_area_count;

	if(language == English)
	{
		cout << "Succeeded to separate.\n\n" << "Current selected areas:\n";
		for(int i = 0; i < sel_area_count; ++i)
			cout << "Selected area #" << i + 1 << ": block " << sel_area_begin[i] + 1 << " - " << sel_area_end[i] + 1 << endl;
	}
	else
	{
		cout << "分离成功。\n\n" << "当前选区：\n";
		for(int i = 0; i < sel_area_count; ++i)
			cout << "选区 #" << i + 1 << "：区块" << sel_area_begin[i] + 1 << " - " << sel_area_end[i] + 1 << endl;
	}
	cout << endl;
}

void merge_area(char* str)
{
	if(!check_loaded())  return;
	
	char str1[10], str2[10];
	int pos = 0;
	while(pos < strlen(str) && str[pos] != ' ')
		++pos;
	strncpy(str1, str, pos);
	str1[pos] = '\0';
	while(pos < strlen(str) && str[pos] == ' ')
		++pos;
	strcpy(str2, str + pos);
	
	char* pEnd;
	int begin = strtol(str1, &pEnd, 10);
	int end = strtol(str2, &pEnd, 10);
	if(begin <= 0 || begin > end || end > sel_area_count)
	{
		if(language == English)
			cout << "Invalid input. Please check your input and try again.\n\n";
		else
			cout << "输入错误。请检查输入后重试。\n\n";
		return;
	}
	
	sel_area_count = sel_area_count - end + begin;
	int block_begin = sel_area_begin[begin - 1];
	int block_end = sel_area_end[begin - 1];
	for(int i = begin; i <= end; ++i)
	{
		if(sel_area_begin[i - 1] < block_begin)
			block_begin = sel_area_begin[i - 1];
		if(sel_area_end[i - 1] > block_end)
			block_end = sel_area_end[i - 1];
	}
	sel_area_begin[begin - 1] = block_begin;
	sel_area_end[begin - 1] = block_end;
	sel_area_begin.erase(sel_area_begin.begin() + begin, sel_area_begin.begin() + end);
	sel_area_end.erase(sel_area_end.begin() + begin, sel_area_end.begin() + end);

	if(language == English)
	{
		cout << "Succeeded to merge.\n\n" << "Current selected areas:\n";
		for(int i = 0; i < sel_area_count; ++i)
			cout << "Selected area #" << i + 1 << ": block " << sel_area_begin[i] + 1 << " - " << sel_area_end[i] + 1 << endl;
	}
	else
	{
		cout << "合并成功。\n\n" << "当前选区：\n";
		for(int i = 0; i < sel_area_count; ++i)
			cout << "选区 #" << i + 1 << "：区块" << sel_area_begin[i] + 1 << " - " << sel_area_end[i] + 1 << endl;
	}
	cout << endl;
}

void modify_area(char* str)
{
	if(!check_loaded())  return;
	
	char str1[10], str2[10], str3[10];
	int pos1 = 0, pos2 = 0;
	while(pos1 < strlen(str) && str[pos1] != ' ')
		++pos1;
	strncpy(str1, str, pos1);
	str1[pos1] = '\0';
	
	while(pos1 < strlen(str) && str[pos1] == ' ')
		++pos1;
	pos2 = pos1;
	while(pos2 < strlen(str) && str[pos2] != ' ')
		++pos2;
	strncpy(str2, str + pos1, pos2 - pos1);
	str2[pos2 - pos1] = '\0';
	
	while(pos2 < strlen(str) && str[pos2] == ' ')
		++pos2;
	strcpy(str3, str + pos2);
	
	char* pEnd;
	int index = strtol(str1, &pEnd, 10);
	int begin = strtol(str2, &pEnd, 10);
	int end = strtol(str3, &pEnd, 10);
	if(begin <= 0 || begin > end || end > block_count || index <= 0 || index > sel_area_count)
	{
		if(language == English)
			cout << "Invalid input. Please check your input and try again.\n\n";
		else
			cout << "输入错误。请检查输入后重试。\n\n";
		return;
	}
	
	sel_area_begin[index - 1] = begin - 1;
	sel_area_end[index - 1] = end - 1;
	if(language == English)
	{
		cout << "Succeeded to modify.\n\n" << "Current selected areas:\n";
		for(int i = 0; i < sel_area_count; ++i)
			cout << "Selected area #" << i + 1 << ": block " << sel_area_begin[i] + 1 << " - " << sel_area_end[i] + 1 << endl;
	}
	else
	{
		cout << "修改成功。\n\n" << "当前选区：\n";
		for(int i = 0; i < sel_area_count; ++i)
			cout << "选区 #" << i + 1 << "：区块" << sel_area_begin[i] + 1 << " - " << sel_area_end[i] + 1 << endl;
	}
	cout << endl;
}

void preview(char* str)
{
	if(!check_loaded())  return;
	
	char* pEnd;
	int value = strtol(str, &pEnd, 10);
	if(value > sel_area_count || value <= 0)
	{
		if(language == English)
			cout << "Invalid input. Please check your input and try again.\n\n";
		else
			cout << "输入错误。请检查输入后重试。\n\n";
		return;
	}
   
   char preview_name[200], str_value[10];
	wchar_t w_preview_name[200];
	strcpy(preview_name, "__temp__/");
	itoa(value, str_value, 10);
	strcat(preview_name, str_value);
	if(output_format == PNG)
   	strcat(preview_name, ".png");
   else  strcat(preview_name, ".jpg");
	mbstowcs(w_preview_name, preview_name, strlen(preview_name));
	w_preview_name[strlen(preview_name)] = L'\0';
	
	CLSID encoderClsid;
   if(output_format == PNG)
   	GetEncoderClsid(L"image/png", &encoderClsid);
   else
   	GetEncoderClsid(L"image/jpeg", &encoderClsid);
	
	int begin = sel_area_begin[value - 1];
	int end = sel_area_end[value - 1];
	int y_begin = block_Y_begin[begin];
	int y_end = block_Y_end[end];
	int x_begin = block_X_begin[begin];
	int x_end = block_X_end[begin];
	for(int i = begin; i <= end; ++i)
	{
		if(block_X_begin[i] < x_begin)
			x_begin = block_X_begin[i];
		if(block_X_end[i] > x_end)
			x_end = block_X_end[i];
	}
	Bitmap* cur_bitmap = new Bitmap(w_cur_name);
	Bitmap* pre_bitmap = cur_bitmap -> Clone(x_begin, y_begin, x_end - x_begin + 1,
														  y_end - y_begin + 1, PixelFormat24bppRGB);
	
   Status stat = pre_bitmap -> Save(w_preview_name, &encoderClsid, NULL);
	if(access("__temp__", X_OK) != 0)
		system("mkdir __temp__");
	pre_bitmap -> Save(w_preview_name, &encoderClsid, NULL);
	delete cur_bitmap, pre_bitmap;

	char cmd[100] = "start \"\" \"";
	strcat(cmd, preview_name);
	strcat(cmd, "\"");
	system(cmd);
	cout << "\n";
}

void set_merge(char* str)
{
	if(!check_loaded())  return;
	
	char str1[10], str2[10];
	int pos = 0;
	while(pos < strlen(str) && str[pos] != ' ')
		++pos;
	strncpy(str1, str, pos);
	str1[pos] = '\0';
	while(pos < strlen(str) && str[pos] == ' ')
		++pos;
	strcpy(str2, str + pos);
	
	char* pEnd;
	int _h = strtol(str1, &pEnd, 10);
	int _s = strtol(str2, &pEnd, 10);
	if(_h <= 0 || _s <= 0)
	{
		if(language == English)
			cout << "Invalid input. Please check your input and try again.\n\n";
		else
			cout << "输入错误。请检查输入后重试。\n\n";
		return;
	}
	
	merge_height = _h;
	merge_span = _s;
	if(language == English)
		cout << "Success.\n\n";
	else
		cout << "设置成功。\n\n";
	detect_block();
}

void set_tolerance(char* str)
{
	if(!check_loaded())  return;
	
	char* pEnd;
	int value = strtol(str, &pEnd, 10);
	if(value > 255 || value < 0 || (value == 0 && str[0] != '0'))
	{
		if(language == English)
			cout << "Invalid input. Please check your input and try again.\n\n";
		else
			cout << "输入错误。请检查输入后重试。\n\n";
		return;
	}
	
	tolerance = value;
	if(language == English)
		cout << "Successfully set tolerance value to " << tolerance << ".\n\n";
	else
		cout << "容差值已成功设为" << tolerance << "。\n\n";
	
	detect_block();
}

bool check_loaded()
{
	if(!loaded)
	{
		if(language == English)
			cout << "You have to load an image first.\n\n";
		else
			cout << "您需要先载入一张图片。\n\n";
		return false;
	}
	return true;
}

bool check_format(char* str)
{
	char* pch = strrchr(str, '.');
	if(pch == nullptr)
		return false;

	char ext[10];
	strcpy(ext, pch + 1);
	for(int i = 0; i < strlen(ext); ++i)
	{
		if(ext[i] >= 'a' && ext[i] <= 'z')
			ext[i] -= 32;
	}
	
	if(strcmp(ext, "PNG") == 0)
	{
		cur_format = PNG;
		output_format = PNG;
		return true;
	}
	else if(strcmp(ext, "JPG") == 0)
	{
		cur_format = JPG;
		output_format = JPG;
		return true;
	}
	else  return false;
}

void set_name(char* str)
{
	char* pch = strrchr(str, '/');
	if(pch == nullptr)
		pch = strrchr(str, '\\');
	
	if(pch == nullptr)
	{
		strcpy(cur_filename, str);
		strcpy(cur_path, "./");
	}
	else
	{
		strcpy(cur_filename, pch + 1);
		strncpy(cur_path, str, pch + 1 - str);
		cur_path[pch + 1 - str] = '\0';
	}
	
	mbstowcs(w_cur_name, str, strlen(str));
	w_cur_name[strlen(str)] = L'\0';
}

void set_output_name(char* output_name, int index)
{
	strcpy(output_name, output_path);
	int pos_format = 0, pos = strlen(output_name);
	int len = strlen(output_name_format), len_index = 1;
	while(pos_format < len)
	{
		char ch = output_name_format[pos_format];
		if(ch == '%')
		{
			++pos_format;
			if(pos_format == len)
			{
				output_name[pos++] = '%';
				output_name[pos] = '\0';
				break;
			}
			ch = output_name_format[pos_format];
			if(ch == '%')
			{
				output_name[pos++] = '%';
				output_name[pos] = '\0';
				continue;
			}
			else if(ch == 'f')
			{
				strcat(output_name, cur_filename);
				char* pch = strrchr(output_name, '.');
				if(pch != nullptr)
					*pch = '\0';
				pos = strlen(output_name);
				++pos_format;
			}
			else if(ch != '0' && ch != 's')
			{
				output_name[pos++] = '%';
				output_name[pos++] = ch;
				output_name[pos] = '\0';
				++pos_format;
			}
			else
			{
				len_index = 1;
				while(ch == '0')
				{
					++len_index;  ++pos_format;
					if(pos_format == len)
					{
						output_name[pos++] = '%';
						for(int i = 1; i < len_index; ++i)
							output_name[pos++] = '0';
						output_name[pos] = '\0';
						break;
					}
					ch = output_name_format[pos_format];
				}
				if(ch == 's')
				{
					int begin = 1;
					++pos_format;
					if(output_name_format[pos_format] == '+' && output_name_format[pos_format + 1] >= '0'
						&& output_name_format[pos_format + 1] <= '9')
					{
						++pos_format;
						char* pEnd;
						begin = strtol(output_name_format + pos_format, &pEnd, 10) + 1;
						while(output_name_format[pos_format] >= '0' && output_name_format[pos_format] <= '9')
							++pos_format;
					}
					char str[10];
					itoa(index + begin, str, 10);
					if(strlen(str) < len_index)
					{
						for(int i = 0; i < len_index - strlen(str); ++i)
							output_name[pos++] = '0';
					}
					output_name[pos] = '\0';
					strcat(output_name, str);
					pos = strlen(output_name);
				}
				else
				{
					output_name[pos++] = '%';
					for(int i = 1; i < len_index; ++i)
						output_name[pos++] = '0';
					output_name[pos++] = ch;
					output_name[pos] = '\0';
					++pos_format;
				}
			}
		}
		else
		{
			output_name[pos++] = ch;
			output_name[pos] = '\0';
			++pos_format;
		}
	}
	if(output_format == PNG)
   	strcat(output_name, ".png");
   else  strcat(output_name, ".jpg");
}

bool is_background(Color color)
{
	return (color.GetR() >= 0xFF - tolerance && color.GetG() >= 0xFF - tolerance
		  && color.GetB() >= 0xFF - tolerance);
}

void detect_block() 
{
	Bitmap* cur_bitmap = new Bitmap(w_cur_name);
	const int HEIGHT = cur_bitmap -> GetHeight();
	const int WIDTH  = cur_bitmap -> GetWidth();
	
	bool* is_bg_line = new bool[HEIGHT];
	for(int y = 0; y < HEIGHT; ++y)
	{
		Color color;
		is_bg_line[y] = true;
		for(int x = 0; x < WIDTH; ++x)
		{
			cur_bitmap -> GetPixel(x, y, &color);
			if(!is_background(color))
			{
				is_bg_line[y] = false;
				break;
			}
		}
	}
	
	block_X_begin.clear();  block_X_end.clear();
	block_Y_begin.clear();  block_Y_end.clear();
	block_count = 0;
	for(int y = 0; y < HEIGHT; ++y)
	{
		int begin = y, end;
		bool record = false;
		while(y < HEIGHT && !is_bg_line[y])
		{
			record = true;
			end = (y++);
		}
		if(record)
		{
			block_Y_begin.push_back(begin);
			block_Y_end.push_back(end);
			block_count++;
		}
	}
	for(int i = 0; i < block_count; ++i)
	{
		Color color;
		for(int x = 0; x < WIDTH; ++x)
		{
			bool intersect = false;
			for(int y = block_Y_begin[i]; y <= block_Y_end[i]; ++y)
			{
				cur_bitmap -> GetPixel(x, y, &color);
				if(!is_background(color))
				{
					intersect = true;
					break;
				}
			}
			if(intersect)
			{
				block_X_begin.push_back(x);
				break;
			}
		}
		for(int x = WIDTH - 1; x >= 0; --x)
		{
			bool intersect = false;
			for(int y = block_Y_begin[i]; y <= block_Y_end[i]; ++y)
			{
				cur_bitmap -> GetPixel(x, y, &color);
				if(!is_background(color))
				{
					intersect = true;
					break;
				}
			}
			if(intersect)
			{
				block_X_end.push_back(x);
				break;
			}
		}
	}
	delete cur_bitmap;
	delete[] is_bg_line;

	if(block_count == 0)
	{
		if(language == English)
			cout << "No block is detected. You may need to modify the tolerance value.\n";
		else
			cout << "未检测到区块。您可能需要修改容差值。\n";
	} 
	else
	{
		if(language == English)
			cout << block_count << " blocks is/are detected.\n";
		else
			cout << "检测到" << block_count << "个区块。\n";
	}
	auto_merge();
}

void auto_merge()
{
	sel_area_begin.clear();
	sel_area_end.clear();
	sel_area_count = 0;
	int i = 0;
	while(i < block_count)
	{
		int begin = i, end = i;
		while(i < block_count - 1)
		{
			bool b = ((block_Y_end[i] - block_Y_begin[i] < merge_height) ||
						 (block_Y_end[i + 1] - block_Y_begin[i + 1] < merge_height)) &&
						(block_Y_begin[i + 1] - block_Y_end[i] < merge_span);
			if(b)  end = (++i);
			else
			{
				end = i;
				break;
			}
		}
		++i;  ++sel_area_count;
		sel_area_begin.push_back(begin);
		sel_area_end.push_back(end);
	}
	
	if(block_count != 0)
	{
		if(language == English)
		{
			cout << "Current selected areas:\n";
			for(int i = 0; i < sel_area_count; ++i)
				cout << "Selected area #" << i + 1 << ": block " << sel_area_begin[i] + 1 << " - " << sel_area_end[i] + 1 << endl;
		}
		else
		{
			cout << "当前选区：\n";
			for(int i = 0; i < sel_area_count; ++i)
				cout << "选区 #" << i + 1 << "：区块" << sel_area_begin[i] + 1 << " - " << sel_area_end[i] + 1 << endl;
		}
	}
	cout << endl;
}

int GetEncoderClsid(const wchar_t* format, CLSID* pClsid)
// Reference: https://docs.microsoft.com/zh-cn/windows/win32/gdiplus/-gdiplus-retrieving-the-class-identifier-for-an-encoder-use
{
	unsigned int num = 0, size = 0;
   ImageCodecInfo* pImageCodecInfo = NULL;
   GetImageEncodersSize(&num, &size);
   if(size == 0)
		exit(1); 
   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
		exit(2);
   GetImageEncoders(num, size, pImageCodecInfo);

   for(int j = 0; j < num; ++j)
   {
      if(wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;
      }
   }
   free(pImageCodecInfo);
   exit(3);
}

int main(int argc, char** argv)
{
	GdiplusStartupInput gdiplusStartupInput;
   ULONG_PTR gdiplusToken;
   GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
   char null[1] = "";
	about(null);  help(null);
	if(language == English)
		cout << "Please load an image.\n\n";
	else
		cout << "请载入图片。\n\n";

	while(true)
	{
		char str[100], former_part[20], latter_part[100];
		cout << "smc> ";
		cin.getline(str, 100);
		
		char* pch = strchr(str, ' ');
		if(pch == nullptr)
		{
			strcpy(former_part, str);
			strcpy(latter_part, "\0");
		}
		else
		{
			int len = pch - str;
			strncpy(former_part, str, len);
			former_part[len] = '\0';
			while(*pch == ' ')
				++pch;
			strcpy(latter_part, pch);
		}
		
		int i;
		for(i = 0; i < NUM; ++i)
		{
			if(strcmp(former_part, short_command[i]) == 0 || strcmp(former_part, long_command[i]) == 0 )
			{
				(*functions[i]) (latter_part);
				break;
			}
		}
		if(i == NUM && str[0] != '\0')
		{
			if(language == English)
				cout << "Wrong command. Please check your input and try again.\n\n";
			else
				cout << "错误的指令。请检查输入后重试。\n\n";
		}
	}
	GdiplusShutdown(gdiplusToken);
	return 0;
}
