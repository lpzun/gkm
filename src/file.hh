#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

void remove_comments(istream& in, ostream& out, const string& comment) {
string line;
while (getline(in, line = "")) {
  const size_t comment_start = line.find(comment);
  out << ( comment_start == string::npos ? line : line.substr(0, comment_start) ) << endl; }}

void remove_comments(istream& in, const string& filename, const string& comment) {
ofstream out(filename.c_str());
remove_comments(in, out, comment); }

void remove_comments(const string& in, string& out, const string& comment) {
std::istringstream  instream(in);
std::ostringstream outstream;
remove_comments(instream, outstream, comment);
out = outstream.str(); }


string removeComment(string fileName)
{

	ifstream file;

	ifstream orig(fileName.c_str());
	if (!orig) {
		cout << "unable to open file\n";
		exit(1); // terminate with error
	}
	remove_comments(orig, fileName + ".no_comment", "//"); orig.close();
	return (fileName + ".no_comment");
};
