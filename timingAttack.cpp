#include <string>
#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>

#include <inttypes.h>
#include <math.h>
#include <stdio.h>
using namespace std;

bool password_ok (const string & pwd);

static __inline__ uint64_t rdtsc()
{
	uint32_t hi, lo;
	__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
	return ((uint64_t)lo) | (((uint64_t)hi) << 32);
}

struct letterStats {
	char letter;
	uint64_t sum;
	uint64_t sqr;
	double mean;
	double variance;
	double sd;
	double minRange;
	double maxRange;
} ;

int main()
{
	bool isSuccess = false;
	
	string pwd_correct = "";
	
	for(int x = 0; x < 10000; x++)
	{
		//const char *c = &letters[x%26];
		password_ok ("test");
	}
	while(!isSuccess)
	{
		string N_max_string;
		cout << "Please enter N value (larger N if there is confidence interval overlap): " << endl;
		getline(cin, N_max_string);
		int N_max;
		std::istringstream(N_max_string) >> N_max;
		char letters[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
		std::random_shuffle(letters, letters+26);
		
		//setup map
		std::map<char,letterStats> charMap;
		for(int i = 0; i < 26; i++)
		{
			letterStats stat;
			stat.letter = letters[i];
			stat.sum = 0;
			stat.sqr = 0;
			stat.mean = 0;
			stat.variance = 0;
			stat.sd = 0;
			stat.minRange = 0;
			stat.maxRange = 0;
			charMap.insert ( std::pair<char,letterStats>(letters[i], stat));
		}
		
		uint64_t start = 0;
		uint64_t end = 0;

		for(int n = 0; n < N_max; n++)
		{
			std::random_shuffle(letters, letters+26);
			//warmup
			for(int x = 0; x < 100; x++)
			{
				password_ok ("warmuptest");
			}

			//measure time
			for(int i = 0; i < 26; i++)
			{
				string psw_attempt = pwd_correct + letters[i];
				start = rdtsc();
				password_ok (psw_attempt);
				end = rdtsc();
				charMap[letters[i]].sum += end-start;
				charMap[letters[i]].sqr += (end-start)*(end-start);
			}
			
			//shows some progress
			if(n%100000 == 0)
			{
				cout << ".";
				std::cout.flush();
			}
			
		}
		cout << endl;
		typedef map<char, letterStats>::iterator it_type;
		for(it_type iterator = charMap.begin(); iterator != charMap.end(); iterator++) {
			letterStats stat = iterator->second;
			stat.mean = double(stat.sum) / N_max;
			stat.variance = double(stat.sqr + 2*stat.mean*stat.sum + N_max*stat.mean) / (N_max-1);
			stat.sd = sqrt(stat.variance/N_max);
			stat.minRange = stat.mean - double(1.96*stat.sd)/sqrt(N_max);
			stat.maxRange = stat.mean + double(1.96*stat.sd)/sqrt(N_max);
			printf ("%c -  sum: %u minRange: %.4f maxRange: %.4f \n", stat.letter, stat.sum, stat.minRange, stat.maxRange);
		}

		cout << "Please enter probable password sequence: " << endl;
		getline(cin, pwd_correct);
		isSuccess = password_ok (pwd_correct);
	}

	cout << "OK, security breached" << endl;
    return 0;
}

bool password_ok (const string & pwd)
{    
	return pwd == "haha";
}

