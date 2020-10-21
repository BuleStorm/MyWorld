#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <iostream>
#include <string.h>

class LoginWidget
{
public:
	static LoginWidget* getInstance();
	void drawLogin();


	int select_save = -1;
	std::string save1 = "null";
	std::string save2 = "null";
	std::string save3 = "null";

	int select_name = -1;
	std::string map_name_str = "";
	std::string userName = "";
private:
	LoginWidget();
	LoginWidget(const LoginWidget&) = delete;
	LoginWidget& operator = (const LoginWidget&) = delete;

	void drawUI();
	void keyPress();
	void selectInput();
	void drawInput();
private:
	enum class SelectEditLine
	{
		UserName,
		PassWord,
		NoSelect,
	};


	enum class LoginType
	{
		LoginIn,
		LoginUp,
		SelectSave,
		SaveName,
		In
	};

	LoginType loginType = LoginType::LoginIn;
	SelectEditLine sel = SelectEditLine::UserName;



	int select_login = -1;
	std::string passWord = "";
	std::string sUserName = "";
	std::string sPassWord = "";
	std::string hidePassWord = "";
};
#endif // LOGINWIDGET_H
