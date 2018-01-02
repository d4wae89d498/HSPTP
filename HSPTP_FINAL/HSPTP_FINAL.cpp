#include <iostream>
#include <sstream>
#include <string>
#include <vector>
//Warning : OS-Specific header
#include <conio.h>

#include "base64.h"


using namespace std;

struct poly {
	int a = 1;
	int b = 1;
	int c = 0;
};

struct ra {
	int delta = 0;
	int x1 = 0;
	int x2 = 0;
};

poly strToPoly(string pass) {
	poly p;
	int x = 0;
	int l = 0;
	p.c = pass.length();
	p.b = pass.length();
	while (x < pass.length()) {
		l = (int)pass[x];
		//cout << pass[x] << " (" << l << ") " << endl;
		if (l % 97) {
			p.a += 1;
		}
		//p.b += l;
		if (l % 89) {
			p.c += 1;
		}
		x++;
	}

	p.a = (p.a % 10) + 1;
	p.c = p.c % 10 + 1;

	if ((p.c) < 0) {
		p.b += 4 * p.a * (p.c * (-1));
	}
	else {
		p.b += 4 * p.a * (p.c * 1);

	}
	p.b = p.b + 3 * (p.b);


	return p;
}

int f(int x, poly poly) {
	//cout << "f x : " << x << endl;
	return poly.a * (x * x) + (poly.b * x) + (poly.c);
}

ra getRacines(poly p, int fx = 0) {
	ra ra;
	poly t = p;
	t.c -= fx;

	ra.delta = (t.b * t.b) - 4 * (t.a * t.c);
	if (ra.delta < 0) {
		std::cout << "ERROR/ negative delta" << endl;
	}
	else {
		if (ra.delta == 0) {
			ra.x1 = ra.x2 = -t.b / (2 * t.a);
		}
		else {
			ra.x1 = ((-t.b) - sqrt(ra.delta)) / (2 * t.a);
			ra.x2 = ((-t.b) + sqrt(ra.delta)) / (2 * t.a);

			cout << "x1: " << ra.x1 << " x2:" << ra.x2 << endl;

		}
	}


	return ra;
}

string Encrypt(string Text, string pass) {
	poly poly = strToPoly(pass);
	string output;
	int x = 0;
	while (x < Text.length()) {
		output = output + to_string(f((int)Text[x], poly)) + "|";
		x++;
	}
	//cout << "before base64 : " << output << endl;
	char* data = (char*)output.c_str();
	int data_length = strlen(data);
	int encoded_data_length = (int) Base64encode_len(data_length);
	char *base64_string = (char*)malloc(encoded_data_length);

	Base64encode(base64_string, data, data_length);
	output = string(base64_string);
	// "base64_string" is a null terminated string that is an encoding of the
	// binary data pointed to by "data"
	return output;
}

string Decrypt(string Text, string pass) {
	char *base64_string = (char *) Text.c_str();
	char* data = NULL;
	int data_length = 0;
	int alloc_length = Base64decode_len(base64_string);
	data = (char*) malloc(alloc_length);
	data_length = Base64decode(data, base64_string);

	Text = string(data).c_str();
	//cout << "after base64_decode : " << data << endl;

	//Base64::Decode(Text, &Text);
	poly poly = strToPoly(pass);
	/*
	std::ostringstream oss;
oss << text << i;
std::cout << oss.str();
	*/
	string str = "";
	ostringstream current_char_buff;
	ra racines;
	int x = 0;
	while (x < Text.length()) {
		if (Text[x] == '|') {
			cout << current_char_buff.str() << endl;
			racines = getRacines(poly, stoi(current_char_buff.str()));
			if (racines.x1 > racines.x2) {
				str = str + (char)racines.x1;

			}else{
				str = str + (char)racines.x2;
			}
			current_char_buff.str("");
		}
		else {
			current_char_buff << (Text[x]);
			cout << "current_char_buff" << current_char_buff.str() << endl;
		}
		x++;
	}
	return str;
}

void Stop() {
	goto inp;
	inp:
	string in;
	cout << "Enter pass for both encrypt / decrypt : " << endl;
	getline(cin, in);
	poly p = strToPoly(in);
	cout << "a : " << p.a << " b : " << p.b << " c: " << p.c << endl;
	cout << "Enter a string : " << endl;
	string clear;
	string enc;
	getline(cin, clear);
	cout << "Encrypted str : " << endl;
	cout << Encrypt(clear, in) << endl;

	cout << "Crypted str : " << endl;
	getline(cin, enc);
	cout << Decrypt(enc, in) << endl;


	goto inp;
	cout << "[ Press a key to exit ... ]" << endl;
	_getch();
}

int main() {
	Stop();
	return 0;
}