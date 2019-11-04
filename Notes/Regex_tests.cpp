#include <iostream>
#include <chrono>
#include <regex>

int main()
{
	/*/
	while (true)
	{
	/
		uint64_t n = 0;
		std::cout << "Enter a number to sum up to: ";
		std::cin >> n;
		std::cout << "\n";

		uint64_t sumSequential = 0;
		uint64_t sumGaussian = 0;

		std::chrono::microseconds microseconds = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
		std::cout << (uint64_t)microseconds.count() << "\n";

		std::chrono::microseconds timeStartSequential;
		std::chrono::microseconds timeEndSequential;
		std::chrono::microseconds timeStartGaussian;
		std::chrono::microseconds timeEndGaussian;

		//std::cout << localtime_s(nullptr, nullptr) << "\n";
		//
		timeStartSequential = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());

		for (uint64_t i = 0; i < n; i++)
		{
			sumSequential += i + 1;
		}

		timeEndSequential = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
		/

		timeStartGaussian = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());

		sumGaussian = (n * (n + 1)) * 0.5;

		timeEndGaussian = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());

		//
		if (sumSequential != sumGaussian)
		{
			std::cerr << "Sums are not equal!";
			return -1;
		}
		/

		//std::chrono::microseconds differenceSequential = timeEndSequential - timeStartSequential;
		std::chrono::microseconds differenceGaussian = timeEndGaussian - timeStartGaussian;

		std::cout << "The sum was: " << sumGaussian << "\n";

		//std::cout << "Time sequential: " << differenceSequential.count() << "\n";
		std::cout << "Time gaussian: " << differenceGaussian.count() << "\n";
		std::cout << timeStartGaussian.count() << ", " << timeEndGaussian.count() << "\n\n";
	//}
	*/

	std::string test_script = R"(
CALL ANONF TAKES int num -> str AS
	SET rv TO ""
	FOR i OF num DO
		SET rv TO rv + "n"
RETURN rv WITH 12

SET b TO 12

//SCOPES !!!!!!!!
SET a TO CALL ANONF TAKES int num -> str AS
	SET rv TO ""
	FOR i OF num DO
		SET rv TO rv + "n"
RETURN rv WITH b

STRUCT vec3 AS
	float x
	float y
	float z
END STRUCT

STRUCT quat AS
	float x
	float y
	float z
	float w
END STRUCT

STRUCT lightning AS
	vec3 position
	quat direction
	REAL brightness
	REAL damage_potential
END STRUCT

STRUCT blueElephant AS
	INT alpha
	STR beta
	LIST gamma
	lightning omega
END STRUCT

SET b TO CREATE blueElephant WITH 1, "2", [3, 4, 5], (CREATE lightning WITH (CREATE vec3 WITH 0, 0, 0), (CREATE quat WITH 0, 0, 0, 0), 10.63, 6365.342)
	)";
	
	std::regex struct_regex("(STRUCTURE|STRUCT) ([a-zA-Z_][a-zA-Z0-9_]*) AS *?\n+?((\t| )*([a-zA-Z_][a-zA-Z0-9_]*) ([a-zA-Z_][a-zA-Z0-9_]*) *?\n)*? *?\n*?(\t| )*?END (STRUCTURE|STRUCT)");
	
	std::cout << std::regex_match(test_script, struct_regex) << '\n';
	std::smatch match;
	std::cout << std::regex_match(test_script, match, struct_regex) << '\n';

	//std::cout << match[1] << '\n';

	std::cmatch m;
	bool s = std::regex_search(test_script.c_str(), m, struct_regex);
	std::cout << s << '\n';
	std::cout << "m[0]: " << m[0] << '\n';
	std::cout << "m[1]: " << m[1] << '\n';
	std::cout << "m[2]: " << m[2] << '\n';
	std::cout << "m[3]: " << m[3] << '\n';

	std::regex re("STRUCT");
	std::cmatch m2;
	bool success = std::regex_search(test_script.c_str(), m2, re);

	std::cout << success << '\n';
	std::cout << "m2[0]: " << m2[0] << "\n";
	std::cout << "m2: " << test_script[m2.prefix().length()] << '\n';
	std::cout << "m2: " << m2.suffix() << '\n';
	system("PAUSE");
	return 0;
}