#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// 1048576 -> 1 megabyte
constexpr uint64_t max_size = 1048576 * 512;

char toLowerCase(const char &c)
{
	if (c >= 'A' && c <= 'Z') {
		return c + 'a' - 'A';
	}
	else {
		return c;
	}
}

int main()
{
	string filename;
	string input_size;

	string size_unit;
	uint64_t size = 0;

	cout << "Type file name: ";
	getline(cin, filename);

	do {
		cout << "Type file size: ";
		getline(cin, input_size);

		stringstream stream(input_size);
		stream >> size;

		if (stream.fail()) {
			cout << endl << "Invalid input!" << endl << endl;
		}
		else {
			if (size == 0) {
				break;
			}
			else {
				stream >> size_unit;

				char c = '\0';
				if (size_unit.empty()) {
					c = 'b';
				}
				else if (size_unit.size() == 2 && toLowerCase(size_unit[1]) == 'b') {
					c = toLowerCase(size_unit[0]);
				}

				if (c != 'b' && c != 'k' && c != 'm' && c != 'g' && c != 't') {
					cout << endl << "Invalid input!" << endl << endl;
				}
				else {
					bool to_large = false;

					if (c == 'k' || c == 'm' || c == 'g' || c == 't') {
						if (size * 1024 > size) {
							size *= 1024;
							if (c == 'm' || c == 'g' || c == 't') {
								if (size * 1024 > size) {
									size *= 1024;
									if (c == 'g' || c == 't') {
										if (size * 1024 > size) {
											size *= 1024;
											if (c == 't') {
												if (size * 1024 > size) {
													size *= 1024;
												}
												else {
													to_large = true;
												}
											}
										}
										else {
											to_large = true;
										}
									}
								}
								else {
									to_large = true;
								}
							}
						}
						else {
							to_large = true;
						}
					}

					if (to_large) {
						cout << endl << "File size too large!" << endl << endl;
					}
					else {
						break;
					}
				}
			}

		}
	} while (true);

	ofstream file(filename, ios::out | ios::binary);
	if (file.is_open()) {
		cout << endl << "Generating content...";

		char *content = nullptr;
		if (size > max_size) {
			content = (char *)calloc((size_t)max_size, sizeof(char));
		}
		else {
			content = (char *)calloc((size_t)size, sizeof(char));
		}

		if (content != nullptr) {
			cout << " Done!" << endl;

			uint64_t times = size / max_size;
			uint64_t rest = size % max_size;

			cout << "Writing content...";
			for (uint64_t i = 0; i < times && !file.bad(); ++i) {
				file.write(content, max_size);
			}
			if (rest > 0) {
				file.write(content, rest);
			}

			free(content);

			if (!file.bad()) {
				cout << " Done!" << endl;
			}
			else {
				cout << " ERROR! (Something went wrong...)" << endl;
			}

		}
		else {
			cout << " ERROR! (Unable to allocate enough memory)" << endl;
		}

		file.close();
	}

	return 0;
}