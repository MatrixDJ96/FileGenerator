#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
	string filename;
	string ssize;

	cout << "Type file name: ";
	getline(cin, filename);
	cout << "Type file size (integer): ";
	getline(cin, ssize);

	ofstream file(filename);
	if (file.is_open())
	{
		// 1048576 -> 1 megabyte
		uint64_t max_size = 1048576 * 512;

		size_t index = ssize.size() - 1;
		uint32_t base_size = strtoul(ssize.c_str(), NULL, 10);
		char unit = tolower(ssize[index]);

		bool b = false;
		do
		{
			if (index > 0) index--;
			else break;
			if (unit == ' ' || unit == 'b')
			{
				if (!b) b = true;
				else break;
				unit = tolower(ssize[index]);
			}
		} while (unit == ' ' || unit == 'b');

		uint64_t size = base_size;
		if (unit == 'k' || unit == 'm' || unit == 'g' || unit == 't')
		{
			size *= 1024;
			if (unit == 'm' || unit == 'g' || unit == 't')
			{
				size *= 1024;
				if (unit == 'g' || unit == 't')
				{
					size *= 1024;
					if (unit == 't')
					{
						size *= 1024;
					}
				}
			}
		}

		cout << "Generating content...";
		char* content = NULL;
		if (size > max_size)
		{
			content = new char[max_size];
			for (uint64_t i = 0; i < max_size; ++i)
			{
				content[i] = '\0';
			}
		}
		else
		{
			content = new char[size];
			for (uint64_t i = 0; i < size; ++i)
			{
				content[i] = '\0';
			}
		}
		cout << " Done!\n";

		uint32_t times = size / max_size;
		uint32_t rest = size % max_size;

		cout << "Writing content...";
		for (uint32_t i = 0; i < times; ++i)
		{
			file.write(content, max_size);
		}
		if (rest > 0)
		{
			file.write(content, rest);
		}
		cout << " Done!\n";

		file.close();
	}

	return 0;
}