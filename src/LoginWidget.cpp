#include "LoginWidget.h"

#include "GLWidget.h"
#include "Interface.h"
#include "Control.h"
#include "OperationDB.h"


static Interface *interFace = Interface::getInstance();
static GLWidget* glWindow = GLWidget::getInstance();
static Control* control = Control::getInstance();
static OperationDB* operationDB = OperationDB::getInstance();

LoginWidget* LoginWidget::getInstance()
{
	static LoginWidget loginWidget;
	return &loginWidget;
}

void LoginWidget::drawLogin()
{
	while (!glfwWindowShouldClose(glWindow->getWindow()) && loginType != LoginType::In)
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		drawUI();
		drawInput();

		glfwSwapBuffers(glWindow->getWindow());
		glfwPollEvents();

		selectInput();
		keyPress();
	}
}

LoginWidget::LoginWidget()
{
}

void LoginWidget::drawUI()
{
	switch (loginType)
	{
	case LoginWidget::LoginType::LoginIn:
		interFace->drawText("My World Game", Width / 2 - 150, Height - 80, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));
		if(select_login == 0)
			interFace->drawText("UserName:", Width / 2 - 400, Height / 2 + 50, 2.0f, glm::vec3(0.11f, 0.56f, 1.00f));
		else
			interFace->drawText("UserName:", Width / 2 - 400, Height / 2 + 50, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));

		if (select_login == 1)
			interFace->drawText("PassWord:", Width / 2 - 400, Height / 2 - 50, 2.0f, glm::vec3(0.11f, 0.56f, 1.00f));
		else
			interFace->drawText("PassWord:", Width / 2 - 400, Height / 2 - 50, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));

		if(select_login == 2)
			interFace->drawText("Sign in", Width / 2 - 250, 100, 2.0f, glm::vec3(0.11f, 0.56f, 1.00f));
		else
			interFace->drawText("Sign in", Width / 2 - 250, 100, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));

		if(select_login == 3)
			interFace->drawText("Sign up", Width / 2 + 150, 100, 2.0f, glm::vec3(0.11f, 0.56f, 1.00f));
		else
			interFace->drawText("Sign up", Width / 2 + 150, 100, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));

		break;
	case LoginWidget::LoginType::LoginUp:
		interFace->drawText("Sign up", Width / 2 - 50, Height - 80, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));
		if (select_login == 0)
			interFace->drawText("UserName:", Width / 2 - 400, Height / 2 + 50, 2.0f, glm::vec3(0.11f, 0.56f, 1.00f));
		else
			interFace->drawText("UserName:", Width / 2 - 400, Height / 2 + 50, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));

		if (select_login == 1)
			interFace->drawText("PassWord:", Width / 2 - 400, Height / 2 - 50, 2.0f, glm::vec3(0.11f, 0.56f, 1.00f));
		else
			interFace->drawText("PassWord:", Width / 2 - 400, Height / 2 - 50, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));

		if (select_login == 2)
			interFace->drawText("OK", Width / 2 - 250, 100, 2.0f, glm::vec3(0.11f, 0.56f, 1.00f));
		else
			interFace->drawText("OK", Width / 2 - 250, 100, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));

		if (select_login == 3)
			interFace->drawText("Cancel", Width / 2 + 150, 100, 2.0f, glm::vec3(0.11f, 0.56f, 1.00f));
		else
			interFace->drawText("Cancel", Width / 2 + 150, 100, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));
		break;
	case LoginWidget::LoginType::SelectSave:
		interFace->drawText("Game Save", Width / 2 - 100, Height - 80, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));
		if (select_save == 0)
		{
			interFace->drawText("Save 1:", Width / 2 - 350, Height - 170, 2.0f, glm::vec3(0.11f, 0.56f, 1.00f));
			interFace->drawText(save1.c_str(), Width / 2 - 50, Height - 170, 2.0f, glm::vec3(0.11f, 0.56f, 1.00f));
		}
		else
		{
			interFace->drawText("Save 1:", Width / 2 - 350, Height - 170, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));
			interFace->drawText(save1.c_str(), Width / 2 - 50, Height - 170, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));
		}

		if (select_save == 1)
		{
			interFace->drawText("Save 2:", Width / 2 - 350, Height - 270, 2.0f, glm::vec3(0.11f, 0.56f, 1.00f));
			interFace->drawText(save2.c_str(), Width / 2 - 50, Height - 270, 2.0f, glm::vec3(0.11f, 0.56f, 1.00f));
		}
		else
		{
			interFace->drawText("Save 2:", Width / 2 - 350, Height - 270, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));
			interFace->drawText(save2.c_str(), Width / 2 - 50, Height - 270, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));
		}

		if (select_save == 2)
		{
			interFace->drawText("Save 3:", Width / 2 - 350, Height - 370, 2.0f, glm::vec3(0.11f, 0.56f, 1.00f));
			interFace->drawText(save3.c_str(), Width / 2 - 50, Height - 370, 2.0f, glm::vec3(0.11f, 0.56f, 1.00f));
		}
		else
		{
			interFace->drawText("Save 3:", Width / 2 - 350, Height - 370, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));
			interFace->drawText(save3.c_str(), Width / 2 - 50, Height - 370, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));
		}

		if (select_save == 3)
		{
			interFace->drawText("NEW Game", Width / 2 - 330, Height - 470, 2.0f, glm::vec3(0.11f, 0.56f, 1.00f));
		}
		else
		{
			interFace->drawText("NEW Game", Width / 2 - 330, Height - 470, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));
		}

		if (select_save == 4)
		{
			interFace->drawText("LAN Game", Width / 2 + 30, Height - 470, 2.0f, glm::vec3(0.11f, 0.56f, 1.00f));
		}
		else
		{
			interFace->drawText("LAN Game", Width / 2 + 30, Height - 470, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));
		}

		break;
	case LoginWidget::LoginType::SaveName:
		interFace->drawText("Input Save Map Name", Width / 2 - 250, Height - 80, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));
		if (select_name == 0)
			interFace->drawText("Save Map Name:", Width / 2 - 450, Height / 2 + 0, 2.0f, glm::vec3(0.11f, 0.56f, 1.00f));
		else
			interFace->drawText("Save Map Name:", Width / 2 - 450, Height / 2 + 0, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));

		if(select_name == 1)
			interFace->drawText("OK", Width / 2 - 50, Height - 450, 2.0f, glm::vec3(0.11f, 0.56f, 1.00f));
		else
			interFace->drawText("OK", Width / 2 - 50, Height - 450, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));

		break;
	case LoginWidget::LoginType::In:
		break;
	default:
		break;
	}
}

void LoginWidget::keyPress()
{
	static char keyPrees = '^';
	static char oldPreee = keyPrees;
	keyPrees = control->keyPressLogin();
	if (keyPrees != '^' && oldPreee != keyPrees)
	{
		if (loginType == LoginWidget::LoginType::SaveName)
		{
			if (keyPrees == '&')
			{
				if (map_name_str.length() > 0)
					map_name_str.pop_back();
			}
			else
			{
				map_name_str.push_back(keyPrees);
			}
		}

		switch (sel)
		{
		case SelectEditLine::UserName:
			switch (loginType)
			{
			case LoginWidget::LoginType::LoginIn:
				if (keyPrees == '&')
				{
					if (userName.length() > 0)
						userName.pop_back();
				}
				else
				{
					userName.push_back(keyPrees);
				}
				break;
			case LoginWidget::LoginType::LoginUp:

				if (keyPrees == '&')
				{
					if (sUserName.length() > 0)
						sUserName.pop_back();
				}
				else
				{
					sUserName.push_back(keyPrees);
				}
				break;
			case LoginWidget::LoginType::SaveName:
			default:
				break;
			}
			break;
		case SelectEditLine::PassWord:
			switch (loginType)
			{
			case LoginWidget::LoginType::LoginIn:
				if (keyPrees == '&')
				{
					if (passWord.length() > 0)
						passWord.pop_back();
				}
				else
				{
					passWord.push_back(keyPrees);
				}
				break;
			case LoginWidget::LoginType::LoginUp:
				if (keyPrees == '&')
				{
					if (sPassWord.length() > 0)
						sPassWord.pop_back();
				}
				else
				{
					sPassWord.push_back(keyPrees);
				}
				break;
			default:
				break;
			}
			break;
		case SelectEditLine::NoSelect:
			break;
		default:
			break;
		}
	}
	oldPreee = keyPrees;
}

void LoginWidget::selectInput()
{
#if FULLSCREEN
	static bool isRightPress = true;
	static bool isLeftPress = true;
	int x = control->moustX;
	int y = control->moustY;

	if (control->mousePress)
	{
		if (y > 360 && y < 460)
		{
			sel = SelectEditLine::UserName;
		}
		else if (y > 560 && y < 660)
		{
			sel = SelectEditLine::PassWord;
		}
		else
		{
			sel = SelectEditLine::NoSelect;
		}


		if (isLeftPress)
		{
			switch (loginType)
			{
			case LoginType::SelectSave:

				if (y > 230 && y < 350)
				{
					if (save1 != "null")
					{
						select_save = 0;
						loginType = LoginType::In;
					}
				}
				else if (y > 440 && y < 550)
				{
					if (save2 != "null")
					{
						select_save = 1;
						loginType = LoginType::In;
					}
				}
				else if (y > 640 && y < 730)
				{
					if (save3 != "null")
					{
						select_save = 2;
						loginType = LoginType::In;
					}
				}
				else if (x > 280 && x < 760 && y > 820 && y < 920)
				{
					select_save = 3;
					loginType = LoginType::SaveName;
				}
				else if (x > 1000 && x < 1450 && y > 820 && y < 920)
				{
					select_save = 4;
					loginType = LoginType::In;
				}
				break;
			case LoginType::SaveName:

				if (y > 780 && y < 880 && map_name_str != "")
				{
					loginType = LoginType::In;
				}
				break;
			}

		}

		if (x > 460 && x < 760 && y > 800 && y < 900)
		{
			if (isLeftPress)
			{
				isLeftPress = false;
				switch (loginType)
				{
				case LoginWidget::LoginType::LoginIn:
					if (operationDB->comparedUserPass(userName, passWord))
					{
						std::list<std::string> saveList = operationDB->selectUserSaveMap(userName);
						int i = 0;
						for (auto item = saveList.begin(); item != saveList.end(); ++item, ++i)
						{
							if (i == 0)
							{
								save1 = item->c_str();
							}
							else if (i == 1)
							{
								save2 = item->c_str();
							}
							else if (i == 2)
							{
								save3 = item->c_str();
							}
						}

						loginType = LoginType::SelectSave;
					}
					break;
				case LoginWidget::LoginType::LoginUp:
					if (sUserName != "" && sPassWord != "")
					{
						operationDB->insertUser(sUserName, sPassWord);
						loginType = LoginType::LoginIn;
					}
					break;
				case LoginWidget::LoginType::SelectSave:
					break;
				default:
					break;
				}

			}
		}

		if (x > 1230 && x < 1560 && y > 800 && y < 900)
		{
			if (isRightPress == true)
			{
				isRightPress = false;
				switch (loginType)
				{
				case LoginWidget::LoginType::LoginIn:
					loginType = LoginType::LoginUp;
					break;
				case LoginWidget::LoginType::LoginUp:
					loginType = LoginType::LoginIn;
					break;
				case LoginWidget::LoginType::SelectSave:
					break;
				default:
					break;
				}
			}
		}

	}

	if (!(x > 460 && x < 760 && y > 800 && y < 900))
	{
		isLeftPress = true;
	}

	if (!(x > 1230 && x < 1560 && y > 800 && y < 900))
	{
		isRightPress = true;
	}


	switch (loginType)
	{
	case LoginWidget::LoginType::LoginIn:
		if (y > 360 && y < 460)
		{
			select_login = 0;
		}
		else if (y > 460 && y < 660)
		{
			select_login = 1;
		}
		else if (x > 460 && x < 760 && y > 800 && y < 900)
		{
			select_login = 2;
		}
		else if (x > 1230 && x < 1560 && y > 800 && y < 900)
		{
			select_login = 3;
		}
		else
			select_login = -1;
		break;
	case LoginWidget::LoginType::LoginUp:
		if (y > 360 && y < 460)
		{
			select_login = 0;
		}
		else if (y > 460 && y < 660)
		{
			select_login = 1;
		}
		else if (x > 460 && x < 760 && y > 800 && y < 900)
		{
			select_login = 2;
		}
		else if (x > 1230 && x < 1560 && y > 800 && y < 900)
		{
			select_login = 3;
		}
		else
			select_login = -1;

		break;
	case LoginWidget::LoginType::SelectSave:
		if (y > 230 && y < 350)
		{
			select_save = 0;
		}
		else if (y > 440 && y < 550)
		{
			select_save = 1;
		}
		else if (y > 640 && y < 730)
		{
			select_save = 2;
		}
		else if (x > 280 && x < 760 && y > 820 && y < 920)
		{
			select_save = 3;
		}
		else if (x > 1000 && x < 1450 && y > 820 && y < 920)
		{
			select_save = 4;
		}
		else
		{
			select_save = -1;
		}
		break;
	case LoginWidget::LoginType::SaveName:
		if (y > 450 && y < 550)
			select_name = 0;
		else if (y > 780 && y < 880)
			select_name = 1;
		else
			select_name = -1;

		break;
	case LoginWidget::LoginType::In:
		break;
	default:
		break;
	}
#else
	static bool isRightPress = true;
	static bool isLeftPress = true;
	int x = control->moustX;
	int y = control->moustY;
	if (control->mousePress)
	{
		if (y > 170 && y < 270)
		{
			sel = SelectEditLine::UserName;
		}
		else if (y > 270 && y < 350)
		{
			sel = SelectEditLine::PassWord;
		}
		else
		{
			sel = SelectEditLine::NoSelect;
		}


		if (isLeftPress)
		{
			switch (loginType)
			{
			case LoginType::SelectSave:
				if (y > 120 && y < 190)
				{
					if (save1 != "null")
					{
						select_save = 0;
						loginType = LoginType::In;
					}
				}
				else if (y > 220 && y < 280)
				{
					if (save2 != "null")
					{
						select_save = 1;
						loginType = LoginType::In;
					}
				}
				else if (y > 320 && y < 380)
				{
					if (save3 != "null")
					{
						select_save = 2;
						loginType = LoginType::In;
					}
				}
				else if (x > 160 && x < 400 && y > 420 && y < 490)
				{
					select_save = 3;
					loginType = LoginType::SaveName;
				}
				else if (x > 520 && x < 750 && y > 420 && y < 490)
				{
					select_save = 4;
					loginType = LoginType::In;
				}
				break;
			case LoginType::SaveName:
				if (y > 400 && y < 460 && map_name_str != "")
				{
					loginType = LoginType::In;
				}
				break;
			}

		}

		if (x > 230 && x < 400 && y > 410 && y < 480)
		{
			if (isLeftPress)
			{
				isLeftPress = false;
				switch (loginType)
				{
				case LoginWidget::LoginType::LoginIn:
					if (operationDB->comparedUserPass(userName, passWord))
					{
						std::list<std::string> saveList = operationDB->selectUserSaveMap(userName);
						int i = 0;
						for (auto item = saveList.begin(); item != saveList.end(); ++item, ++i)
						{
							if (i == 0)
							{
								save1 = item->c_str();
							}
							else if (i == 1)
							{
								save2 = item->c_str();
							}
							else if (i == 2)
							{
								save3 = item->c_str();
							}
						}

						loginType = LoginType::SelectSave;
					}
					break;
				case LoginWidget::LoginType::LoginUp:
					if (sUserName != "" && sPassWord != "")
					{
						operationDB->insertUser(sUserName, sPassWord);
						loginType = LoginType::LoginIn;
					}
					break;
				case LoginWidget::LoginType::SelectSave:
					break;
				default:
					break;
				}

			}
		}

		if (x > 640 && x < 800 && y > 410 && y < 480)
		{
			if (isRightPress == true)
			{
				isRightPress = false;
				switch (loginType)
				{
				case LoginWidget::LoginType::LoginIn:
					loginType = LoginType::LoginUp;
					break;
				case LoginWidget::LoginType::LoginUp:
					loginType = LoginType::LoginIn;
					break;
				case LoginWidget::LoginType::SelectSave:
					break;
				default:
					break;
				}
			}
		}

	}

	if (!(x > 230 && x < 400 && y > 410 && y < 480))
	{
		isLeftPress = true;
	}

	if (!(x > 640 && x < 800 && y > 410 && y < 480))
	{
		isRightPress = true;
	}


	switch (loginType)
	{
	case LoginWidget::LoginType::LoginIn:
		if (y > 170 && y < 270)
		{
			select_login = 0;
		}
		else if (y > 270 && y < 350)
		{
			select_login = 1;
		}
		else if (x > 230 && x < 400 && y > 410 && y < 480)
		{
			select_login = 2;
		}
		else if (x > 640 && x < 800 && y > 410 && y < 480)
		{
			select_login = 3;
		}
		else
			select_login = -1;
		break;
	case LoginWidget::LoginType::LoginUp:

		if (y > 170 && y < 270)
		{
			select_login = 0;
		}
		else if (y > 270 && y < 350)
		{
			select_login = 1;
		}
		else if (x > 230 && x < 400 && y > 410 && y < 480)
		{
			select_login = 2;
		}
		else if (x > 640 && x < 800 && y > 410 && y < 480)
		{
			select_login = 3;
		}
		else
			select_login = -1;

		break;
	case LoginWidget::LoginType::SelectSave:
		if (y > 120 && y < 190)
		{
			select_save = 0;
		}
		else if (y > 220 && y < 280)
		{
			select_save = 1;
		}
		else if (y > 320 && y < 380)
		{
			select_save = 2;
		}
		else if (x > 160 && x < 400 && y > 420 && y < 490)
		{
			select_save = 3;
		}
		else if (x > 520 && x < 750 && y > 420 && y < 490)
		{
			select_save = 4;
		}
		else
		{
			select_save = -1;
		}
		break;
	case LoginWidget::LoginType::SaveName:
		if (y > 230 && y < 290)
			select_name = 0;
		else if (y > 400 && y < 460)
			select_name = 1;
		else
			select_name = -1;

		break;
	case LoginWidget::LoginType::In:
		break;
	default:
		break;
	}
#endif // FULLSCREEN

}

void LoginWidget::drawInput()
{
	switch (loginType)
	{
	case LoginWidget::LoginType::LoginIn:
		interFace->drawText(userName.c_str(), Width / 2 - 400 + 260, Height / 2 + 50, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));
		hidePassWord.resize(passWord.size(), '*');
		interFace->drawText(hidePassWord.c_str(), Width / 2 - 400 + 260, Height / 2 - 50, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));
		break;
	case LoginWidget::LoginType::LoginUp:
		interFace->drawText(sUserName.c_str(), Width / 2 - 400 + 260, Height / 2 + 50, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));
		interFace->drawText(sPassWord.c_str(), Width / 2 - 400 + 260, Height / 2 - 50, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));
		break;
	case LoginWidget::LoginType::SelectSave:
		break;
	case LoginWidget::LoginType::SaveName:
		interFace->drawText(map_name_str.c_str(), Width / 2 - 50, Height / 2 + 0, 2.0f, glm::vec3(1.00f, 1.00f, 1.00f));
		break;
	case LoginWidget::LoginType::In:
		break;
	default:
		break;
	}
}
