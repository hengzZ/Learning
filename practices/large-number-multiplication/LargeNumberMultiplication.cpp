#include <iostream>
#include <string>

std::string
LargeNumberMultiplication (const std::string str1, const std::string str2)
{
	std::string num1, num2, ret;

	// Determine the number sign
	int negative = 0;
	if ('-'==str1[0]) {
		negative = 1;
	}
	if ('-'==str2[0]) {
		negative = negative ? 0:1; 
	}

	// Get the number part
	if ('-'==str1[0] || '+'==str1[0]) {
		num1 = std::string (str1, 1, str1.size());
	} else {
		num1 = str1;
	}
	if ('-'==str2[0] || '+'==str2[0]) {
		num2 = std::string (str2, 1, str2.size());
	} else {
		num2 = str2;
	}

	// num1 multiplied by num2
	std::string result(num1.size()+num2.size(),0);
	for (int i=num1.size()-1; i>=0; i--) {
		for (int j=num2.size()-1; j>=0; j--) {
			result[i+j+1] += (num1[i]-'0')*(num2[j]-'0');
			result[i+j] += result[i+j+1]/10; // Warrning: updated value maybe larger than 9
			result[i+j+1] %= 10;
		}
	}
	// convert integar to char
	int sPostion = 0;
	for (int i=0; i<result.size(); i++) {
		if (0!=result[i]) {
			sPostion=i;
			break;
		}
	}
	ret = std::string (result, sPostion, result.size());
	for (int i=0; i<ret.size(); i++) {
		ret[i] += '0';
	}

	if (negative) {
		ret = "-"+ret;
	}
	return ret;
}


int
main (int argc, char** argv)
{
	std::string num1, num2;

	std::cin >> num1;
	std::cin >> num2;
	std::string result = LargeNumberMultiplication (num1, num2);

	std::cout << result << std::endl;

	return 0;
}
