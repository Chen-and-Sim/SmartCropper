#ifndef SMARTCROPPER_H
#define SMARTCROPPER_H

#include <vector>
#pragma comment(lib, "libgdiplus.a")
#include <windows.h>
#include <gdiplus.h>

using namespace std;
using namespace Gdiplus;
const int MAX_FILE = 100;

bool loaded = false;
bool loaded_all = false;
enum Language{English, Chinese};
Language language = English;
enum Format{JPG, PNG};
Format cur_format, output_format;
wchar_t w_cur_name[MAX_FILE][200];
char cur_filename[MAX_FILE][100];
char cur_path[100];
char output_path[100] = "crops\\";
char output_name_format[100];

int image_count;
int block_count;
vector<int> block_Y_begin, block_Y_end;
vector<int> block_X_begin, block_X_end;
vector<int> block_in_image;
int sel_area_count;
vector<int> sel_area_begin, sel_area_end;

int tolerance;
int proportion = 0;
int aspect_hor = 0;
int aspect_ver = 0;
int merge_height = 80;
int merge_span = 30;

void about(char*);
void help(char*);
void switch_language(char*);
void quit(char*);
void save(char*);
void current_settings(char*);
void load_image(char*);
void load_all(char*);
void set_output_format(char*);
void set_name_format(char*);
void set_output_path(char*);
void create_area(char*);
void delete_area(char*);
void separate_area(char*);
void merge_area(char*);
void modify_area(char*);
void preview(char*);
void set_merge(char*);
void set_enlarge(char*);
void set_tolerance(char*);

bool check_loaded();
bool check_format(char*);
void set_name(int, char*);
void set_output_name(char*, int);
bool is_background(Color);
void detect_block();
void auto_merge();
int  GetEncoderClsid(const wchar_t*, CLSID*);

const int NUM = 20;
const char short_command[NUM][3] = {"-a", "-h", "-l", "-q", "-s", "-x", "-i", "-u", "-f", "-n", "-p", "-c", "-d", "-e", "-r", "-m", "-v", "-b", "-g", "-t"};
const char long_command[NUM][10] = {"about", "help",   "language", "quit",     "save",  "settings", "load",    "loadall",  "format",  "name",
												"path",  "create", "delete",   "separate", "merge", "modify",   "preview", "setmerge", "enlarge", "tolerance"};
void (*functions[NUM]) (char*) = {about, help, switch_language, quit, save, current_settings, load_image, load_all,
											 set_output_format, set_name_format, set_output_path, create_area, delete_area, 
											 separate_area, merge_area, modify_area, preview, set_merge, set_enlarge, set_tolerance};
#endif
