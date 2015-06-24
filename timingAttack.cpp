#include <inttypes.h>
#include <string>
#include <iostream>
using namespace std;

bool password_ok (const string & pwd);
string getPwdAttempt (const string & correctPwd);

static __inline__ uint64_t rdtsc()
{
	uint32_t hi, lo;
	__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
	return ((uint64_t)lo) | (((uint64_t)hi) << 32);
}

int main()
{
	uint64_t start;
	uint64_t end;
	uint64_t exec_time;
	string pwd_attempt = "";
	string pwd_correct = "crazylongpasswor";
	bool isSuccess = false;
	while(!isSuccess)
	{
		pwd_attempt = getPwdAttempt(pwd_correct);
		start = rdtsc();
		isSuccess = password_ok (pwd_attempt);
		end = rdtsc();
		exec_time = end - start;
		cout << exec_time << endl;
	}
	cout << "OK" << endl;
    return 0;
}

bool password_ok (const string & pwd)
{    
	return pwd == "crazylongpassword";
}

string getPwdAttempt (const string & pwd_correct)
{
	char randChar = 'a' + (random() % 26);
	return pwd_correct + randChar;
}
