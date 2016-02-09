#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helpers.h"

using namespace std;

bool checksum_correct(const unsigned char *b, size_t len)
{
	unsigned char sum  = 0;

	for (size_t i = 0; i < len; i++) {
		sum += b[i];
	}
	if (sum == 0)
		return true;
	else
		return false;
}

const char *record_type_name(const unsigned char type)
{
	switch (type) {
		case 0x80:	return "THEADR";
		case 0x88:	return "COMMENT";
		case 0x8a:	return "MODEND";
		case 0x8c:	return "EXTDEF";
		case 0x90:	return "PUBDEF";
		case 0x94:	return "LINNUM";
		case 0x96:	return "LNAMES";
		case 0x98:	return "SEGDEF";
		case 0x9a:	return "GRPDEF";
		case 0x9c:	return "FIXUPP";
		case 0xa0:	return "LEDATA";
		case 0xa2:	return "LIDATA";
		case 0xf0:	return "LIBRARY";
	}
	fprintf(stderr, "unknown type %x ", (int)type);
	return "NULL";
}

void print_string(const unsigned char *b, const size_t len)
{
	if (len > 0) {

		char *text = new char[len + 1];

		for (size_t i = 0; i < len; i++)
			text[i] = b[i];

		text[len] = '\0';
		cout << text;
		delete text;
	}
}

static int params_to_str(char *str, char *src, int len)
{
	int retval = 0;


	if (*src == 'q') {

		bool is_ptr = false;

		src++;
		len--;

		while (len > 0) {

			int code = *src;

			switch (code) {

				//Integer
				case 'l': {
					strcat(str, " long");
					retval += 5;
					if (is_ptr) {
						strcat(str, "*");
						retval ++;
					}
					if (len > 1) {
						strcat(str, ",");
						retval++;
					}
					is_ptr = false;
					break;
				}
				case 'i': {

					strcat(str, " int");
					retval += 4;
					if (is_ptr) {
						strcat(str, "*");
						retval ++;
					}
					if (len > 1) {
						strcat(str, ",");
						retval++;
					}
					is_ptr = false;
					break;
				}
				case 's': {
					strcat(str, " short");
					retval += 6;
					if (is_ptr) {
						strcat(str, "*");
						retval ++;
					}
					if (len > 1) {
						strcat(str, ",");
						retval++;
					}
					is_ptr = false;
					break;
				}
				case 'c': {
					strcat(str, " char");
					retval += 5;
					if (is_ptr) {
						strcat(str, "*");
						retval ++;
					}
					if (len > 1) {
						strcat(str, ",");
						retval++;
					}
					is_ptr = false;
					break;
				}
				case 'v': {
					strcat(str, " void");
					retval += 5;
					if (is_ptr) {
						strcat(str, "*");
						retval ++;
					}
					if (len > 1) {
						strcat(str, ",");
						retval++;
					}
					is_ptr = false;
					break;
				}

				//Float
				case 'f': {
					strcat(str, " float");
					retval += 6;
					if (is_ptr) {
						strcat(str, "*");
						retval ++;
					}
					if (len > 1) {
						strcat(str, ",");
						retval++;
					}
					is_ptr = false;
					break;
				}
				case 'd': {
					strcat(str, " double");
					retval += 7;
					if (is_ptr) {
						strcat(str, "*");
						retval ++;
					}
					if (len > 1) {
						strcat(str, ",");
						retval++;
					}
					is_ptr = false;
					break;
				}

				//Singness
				case 'u': {
					strcat(str, " unsigned");
					retval += 9;
					break;
				}
				case 'z': {
					strcat(str, " unsigned");
					retval += 9;
					break;
				}

				//Pointer
				case 'n': {
					is_ptr = true;
					break;
				}

				//Variable arguments
				case 'e': {
					is_ptr = false;
					break;
				}
				default: {
					cout << "Error: on code=" << code << "\n";
				}
			}
			src++;
			len--;
		}

	} else {
		cout << "Error: Parameter String without q";
	}

	return retval;
}

void print_func_sig(const unsigned char *b, const size_t len)
{

	if (len <= 0 || b == NULL)
		return;

	if (read_u8(b) != '@') {
		print_string(b, len);
	} else {

		char *signature = new char[1024];

		if (signature != NULL) {
			int pos = 1;
			char *split;

			split = strchr((char*)b, '$');

			if (split == NULL) {
				cout << "Error: No $ found in function name";
				delete signature;
				return;
			}

			const int name_len = split - (char*)b - 1;

			strncpy(signature, (char*)b + pos, name_len);
			signature[name_len] = '\0';
			strcat(signature, "(");

			const int args_len = params_to_str(signature,
				(char*)b + 1 + name_len + 1,
				len - name_len - 2);

			strcat(signature, ")");

			print_string((const unsigned char*)signature,
				name_len + args_len + 2);

			delete signature;
		}
	}
}

void show_THEADR(const unsigned char *b, const size_t len)
{
	size_t str_len = read_u8(b + 3);

	if (str_len < len - 5) {
		cout << "THEADR:\t";
		print_string(b + 4, str_len);
		cout << "\n";
	}
}

void show_COMMENT(const unsigned char *b, const size_t len)
{
	int type;
	int c_class;

	type = read_u8(b);
	c_class = read_u8(b + 1);

	if (type != 0) {
		printf("COMMENT: 0x%x not implemented\n", type);
		return;
	}

	printf("COMMENT:\t");
	//printf("0x%x 0x%x\t", type, c_class);
	switch (c_class) {
		case 0x00: {
			printf("Translator: ");
			print_string(b + 3, len - 1);
			break;
		}
		case 0xa1: {
			printf("New OMF extension: ");
			print_string(b + 3, read_u8(b + 2) + 1);
			break;
		}
		case 0xe8: {
			printf("Class 0xe8: ");
			print_string(b + 4, read_u8(b + 3));
			printf(" 0x%x ", read_u8(b + len -1));
			printf("0x%x ", read_u8(b + len -0));
			printf("0x%x ", read_u8(b + len +1));
			break;
		}
		case 0xe9: {
			printf("Dependency file: ");
			printf("0x%x ", read_u8(b + 3));
			printf("0x%x ", read_u8(b + 4));
			printf("0x%x ", read_u8(b + 5));
			print_string(b + 7, read_u8(b + 6));
			break;
		}
		case 0xea: {
			if (len == 2)
				printf("Class 0xea: 0x%x", read_u16(b + 2));
			else
				printf("Class 0xea: has unspecified length");
			break;
		}
		case 0xf9: {
			if (len == 2)
				printf("Class 0xf9: 0x%x", read_u16(b + 2));
			else
				printf("Class 0xf9: has unspecified length");
			break;
		}
		case 0xfa: {
			if (len == 4)
				printf("Class 0xfa: 0x%x", read_u32(b + 2));
			else
				printf("Class 0xfa: has unspecified length");
			break;
		}
		default: {
			printf("Class 0x%x NOT IMPLEMENTED", c_class);
		}
	}

	printf("\n");
}

void show_LNAMES(const unsigned char *b, const size_t len)
{
	size_t pos = 3;

	cout << "LNAMES:\t";
	while (pos < len) {
		int str_len = read_u8(b + pos);
		cout << "\"";
		print_string(b + pos + 1, str_len);
		cout << "\",";
		pos += str_len + 1;
	}
	cout << "\n";
}

void show_PUBDEF(const unsigned char * b, const size_t len)
{
	int type = read_u8(b);

	if (type == 0x90) {
		// 16 bit
		size_t pos = 5;
		int bgi = read_u8(b + 3);
		int bsi = read_u8(b + 4);

		cout << hex << showbase;
		cout << "PUBDEF (16):\t";
		cout << " Base Group Index=" << bgi;
		cout << " Base Segment Index=" << bsi;
		if (bsi == 0) {
			// base frame present
			int base_frame = read_u16(b + 5);
			cout << " Base Frame =" << base_frame;
			pos += 2;
		}
		cout << "\n";

		while (pos < len) {
			int str_len = read_u8(b + pos);
			int pub_off = read_u16(b + pos + str_len + 1);
			int type_index = read_u8(b + pos + str_len + 3);
			cout << " Public Offset=" << pub_off;
			cout << " Type Index=" << type_index << "\t";
			print_func_sig(b + pos + 1, str_len);
			pos += str_len + 4;
		}
		cout << "\n";
		cout << dec << noshowbase;

	} else if (type == 0x91) {
		// 32 bit
		cout << "PUBDEF (32):\t32bit is not yet supported\n";
	}
}

void show_LIBRARY(const unsigned char *b, const size_t len)
{
	cout << hex << showbase;
	cout << "LIBRARY:";
	cout << " PageSize=" << len + 3;
	cout << " Dictionary Offset=" << (int)read_u32(b + 3);
	cout << " Dictionary Size=" << (int)read_u16(b + 7);
	cout << " Flags=" << (int)read_u8(b + 9);
	cout << "\n";
	cout << dec << noshowbase;
}

void dump_LEDATA(const unsigned char *b, const size_t len, FILE *fd)
{
	fwrite(b + 6, 1, len - 4, fd);
}

void examine_obj(const unsigned char *b, const size_t flen, char *fname)
{
	size_t pos = 0;
	int sect_nr = 1;

	bool has_fdata = false;
	bool is_lib = false;
	FILE *fdata_bin = NULL;

	int d_len;
	int type;


	while (pos < flen && (type != 0x8a && type != 0x8b)) {
		type = read_u8(b + pos);
		d_len = read_u16(b + pos + 1);
		const char *type_name = record_type_name(type);

		if (type == 0xf0) {
			is_lib = true;
		}

		cout << "Section "<< sect_nr << ":\t";
		cout << hex << showbase;
		cout << type_name << "\t" << d_len;

		if (!is_lib && !checksum_correct(b + pos, d_len + 3)) {
			cout << " checksum: NOT OK";
		}

		cout << "\n";
		cout << dec << noshowbase;

		if (type == 0xa0)
			has_fdata = true;

		sect_nr++;
		pos += d_len + 3;
	}

	if (flen - pos != 0) {

		if (is_lib) {
			cout << "Info: Library has " << flen - pos;
			cout << " Bytes payload\n";
		} else {
			cout << "Warning: file contains " << flen - pos;
			cout << " Bytes of garbage\n";
		}
	}

	if (has_fdata) {
		size_t str_len = strlen(fname);
		char * bin_name = (char*)malloc(str_len);
		strcpy(bin_name, fname);
		bin_name[str_len -3] = 'B';
		bin_name[str_len -2] = 'I';
		bin_name[str_len -1] = 'N';

		fdata_bin = fopen(bin_name, "w+");

		if (fdata_bin == NULL) {
			printf("ERROR: open bin file %s\n", bin_name);
			return;
		}
	}

	pos = 0;
	while (pos < flen) {
		type = read_u8(b + pos);
		d_len = read_u16(b + pos + 1);

		switch (type) {
			case 0x80: show_THEADR(b + pos, d_len); break;
			case 0x88: show_COMMENT(b + pos + 3, d_len - 3); break;
			case 0x90: show_PUBDEF(b + pos, d_len); break;
			case 0x96: show_LNAMES(b + pos, d_len); break;
			case 0xa0: {
				dump_LEDATA(b + pos, d_len, fdata_bin);
				break;
			}
			case 0xf0: show_LIBRARY(b + pos, d_len); break;
		}

		sect_nr++;
		pos += d_len + 3;
	}

	if (has_fdata) {
		fclose(fdata_bin);
	}
	return;
}

int main(int argc, char *argv[])
{
	FILE *fd;
	size_t flen;
	unsigned char *buffer;

	if (argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]);
		return -1;
	}

	fd = fopen(argv[1], "rb");
	if (fd != NULL) {
		fseek(fd, 0, SEEK_END);
		flen = ftell(fd);
		fseek(fd, 0, SEEK_SET);

		buffer = (unsigned char*)calloc(flen, sizeof(char));

		fread(buffer, 1, flen, fd);

		fclose(fd);

		examine_obj(buffer, flen, argv[1]);
		free(buffer);


	} else {
		printf("File %s cannot be opened\n", argv[1]);
	}

	return 0;
}
