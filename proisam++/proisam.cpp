

//
// proisam support for c++ code
//

#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <memory>
#include "proisam.hpp"

namespace proisam
{
	bool CATALOG::bCatDefer = false;


	ProisamFile::ProisamFile(const char* filename, int16_t mode) :
		name(filename), nMode(mode)
	{
		std::cout << "ProisamFile::ProisamFile" << std::endl;
		return;
	}

	std::unique_ptr<ProisamFile> ProisamFile::open(const char* filename, int16_t mode) {
		std::unique_ptr<ProisamFile> instance = std::make_unique<ProisamFile>(ProisamFile{ filename, mode });
		std::ifstream infile;
		infile.open(filename, std::ios::binary);
		*instance << infile;
		infile.close();
		return instance;
	}


	std::istream& ProisamFile::operator<<(std::istream& in) {
		if (in.good()) {
			catalog << in;
			return in;
		}
		else {
			throw proisam::ProisamException("file not good");
		}
		return in;
	}




	/*
	* CATALOG methods.
	*/

	std::istream& CATALOG::operator<<(std::istream& in) {
		if (in.good()) {
			/*Read Binary data using streambuffer iterators.*/
			DISKPAGE pagedata;
			DISKCAT data; //intermediate structure
			// TODO: current DiskPage has 1 char of data... why?
			// header is discarded.
			in.read((char*)&pagedata, sizeof(DISKPAGE) - 1);
			in.read((char*)&data, sizeof(DISKCAT));
			if (IS_MAGIC != data.nMagicNo)
			{
				/* Not a ProIsam file */
				throw proisam::ProisamException("file not ProIsam format");
			}
			nKeyLen = data.nKeyLength;
			nPageSz = data.nPageSize;
			nFreeHead = data.nFree;
			nHighLev = data.nHighLevel;

			//load pages
			PAGEBUFF buffer(nPageSz);
			buffer << in;
			return in;
		}
		else {
			throw proisam::ProisamException("file not good");
		}
		return in;
	}


	/*
	* PAGEBUFF 
	*/
	std::istream& PAGEBUFF::operator<<(std::istream& in) {
		if (in.good()) {
			/*Read Binary data using streambuffer iterators.*/
			DISKPAGE pagedata;
			// header is discarded.
			in.read((char*)&pagedata, sizeof(DISKPAGE) - 1);
			in.read((char*)this, nPageSize);
			return in;
		}
		else {
			throw proisam::ProisamException("file not good");
		}
		return in;
	}


}