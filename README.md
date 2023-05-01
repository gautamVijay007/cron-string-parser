# cron-string-parser

Run Console Application:
- Go to the path https://github.com/gautamVijay007/cron-string-parser/tree/main/cron-parser/Debug
- Download cron-parser.exe
- Go to the path where file has been downloaded
- Open Command Promp
- Try running: `cron-parser.exe ＂*/15 0 1,15 * 1-5 /usr/bin/find＂
<img width="394" alt="image" src="https://user-images.githubusercontent.com/132206669/235354923-76a0c21e-d1b1-4dfb-afda-532459380f51.png">

- Try with other valid strings.

Code Walkthrough:
- This is a console cpp application on which Entry point is main function which is at path: https://github.com/gautamVijay007/cron-string-parser/blob/main/cron-parser/cron-parser.cpp
- Our program expects user to pass in a string as an argument.
- Creating object of CronStringParser which handles all the parsing and printing logic for cron data.
- There are two public method in CronStringParser class: parseString(arg[1]) and printData(data)
- ParseString excepts the argument passed from cmd and parses it to list of CronFieldDetails object.
- CronFieldDetails- Details of each field: raw string and type of field
- type of field is handled by Enum class: CronFieldType
- After forming each cron field item in a vector, we iterate through all the items except command and expands those inside expandCronItem(CronFieldDetails cronItem) private method.
- expandCronItem method handles cronItem string validation and expansion logic. After expansion result is returned as a vector of integers to the parent parseString method.
- From there, we return mapping of cronFieldType -> values to the main method. This data we can pass to PrintData method to print data in expected format and order.

Unit Tests:
- Unit tests are added in https://github.com/gautamVijay007/cron-string-parser/blob/main/cron-parser-test/cron-parser-test.cpp
- We can run these tests in visual studio environment.
- Tests are written on top of "CppUnitTest.h" library.
