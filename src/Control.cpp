#include "Control.h"
#include "GLWidget.h"
#include "CommTCP.h"
#include "EliminateAnima.h"
#include "OperationDB.h"

constexpr float moveSpeed = 6;
constexpr float placeSpeed = 0.05;

static CommTCP* commTCP = CommTCP::getInstance();
static Control* control = Control::getInstance();
static GLWidget* glWidget = GLWidget::getInstance();
static CreateWorld* createWorld = CreateWorld::getInstance();
static EliminateAnima* eliminateAnima = EliminateAnima::getInstance();
static OperationDB* operationDB = OperationDB::getInstance();

Control* Control::getInstance()
{
	static Control control;
	return &control;
}

Control::Control()
{
}

void Control::keyPressGame()
{
	windowKey();

	takeCubeKey();
	cameraMoveKey();
	cameraJumpKey();
}

char Control::keyPressLogin()
{
	char key = '^';

	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
		key = 'a';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_B) == GLFW_PRESS)
		key = 'b';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_C) == GLFW_PRESS)
		key = 'c';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
		key = 'd';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_E) == GLFW_PRESS)
		key = 'e';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_F) == GLFW_PRESS)
		key = 'f';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_G) == GLFW_PRESS)
		key = 'g';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_H) == GLFW_PRESS)
		key = 'h';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_I) == GLFW_PRESS)
		key = 'i';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_J) == GLFW_PRESS)
		key = 'j';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_K) == GLFW_PRESS)
		key = 'k';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_L) == GLFW_PRESS)
		key = 'l';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_M) == GLFW_PRESS)
		key = 'm';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_N) == GLFW_PRESS)
		key = 'n';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_O) == GLFW_PRESS)
		key = 'o';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_P) == GLFW_PRESS)
		key = 'p';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_Q) == GLFW_PRESS)
		key = 'q';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_R) == GLFW_PRESS)
		key = 'r';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
		key = 's';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_T) == GLFW_PRESS)
		key = 't';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_U) == GLFW_PRESS)
		key = 'u';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_V) == GLFW_PRESS)
		key = 'v';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
		key = 'w';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_X) == GLFW_PRESS)
		key = 'x';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_Y) == GLFW_PRESS)
		key = 'y';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_Z) == GLFW_PRESS)
		key = 'z';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_1) == GLFW_PRESS)
		key = '1';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_2) == GLFW_PRESS)
		key = '2';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_3) == GLFW_PRESS)
		key = '3';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_4) == GLFW_PRESS)
		key = '4';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_5) == GLFW_PRESS)
		key = '5';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_6) == GLFW_PRESS)
		key = '6';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_7) == GLFW_PRESS)
		key = '7';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_8) == GLFW_PRESS)
		key = '8';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_9) == GLFW_PRESS)
		key = '9';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_0) == GLFW_PRESS)
		key = '0';
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_BACKSPACE) == GLFW_PRESS)
		key = '&';

	return key;
}

void Control::eyeInit(int altitude)
{
	eyePos.y = altitude + EYE_HEIGHT;
	cameraPos.y = eyePos.y;
	eyeAltitude = eyePos.y;
}

void Control::eyeHeight()
{
	if (ps == PeopleStatus::JumpUp)
	{
		eyePos.y += (beginJumpAltitude - eyePos.y) * 0.025 + control->elapseTime;
		cameraPos.y = eyePos.y;
		eyeAltitude = eyePos.y;

		if (eyePos.y >= beginJumpAltitude)
		{
			eyeAltitude = beginJumpAltitude;
			eyePos.y = eyeAltitude;
			cameraPos.y = eyePos.y;
			ps = PeopleStatus::Eecline;
		}
	}
	else if (ps == PeopleStatus::Eecline)
	{
		eyePos.y -= (beginJumpAltitude - eyePos.y) * 0.025 + control->elapseTime;
		cameraPos.y = eyePos.y;
		eyeAltitude = eyePos.y;

		if (eyePos.y <= createWorld->getAltitude(ceil(eyePos.x - 0.5), ceil(eyePos.z - 0.5), ceil(eyePos.y - 1.5)) + EYE_HEIGHT)
		{
			eyeAltitude = createWorld->getAltitude(ceil(eyePos.x - 0.5), ceil(eyePos.z - 0.5), ceil(eyePos.y - 1.5)) + EYE_HEIGHT;
			eyePos.y = eyeAltitude;
			cameraPos.y = eyePos.y;
			ps = PeopleStatus::Normal;
		}
	}
	else if (isCollision(eyePos.x, eyePos.y - 1, eyePos.z))
	{
		ps = PeopleStatus::Eecline;
		beginJumpAltitude = eyePos.y;
	}
}

void mousePress(GLFWwindow* window, int button, int action, int mods)
{
	if (control->isLogin)
	{
		if (action == GLFW_PRESS)
		{
			switch (button)
			{
			case GLFW_MOUSE_BUTTON_RIGHT:
				control->destroyBlock();
				break;
			case GLFW_MOUSE_BUTTON_LEFT:
				control->placeBlock();

				if (!control->isCursorDisabled)
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				break;
			default:
				return;
			}
		}
	}
	else
	{
		if (action == GLFW_PRESS)
		{
			switch (button)
			{
			case GLFW_MOUSE_BUTTON_LEFT:
				control->mousePress = true;
				break;
			default:
				return;
			}
		}
		else
		{
			control->mousePress = false;
		}
	}
}

void Control::placeBlock()
{
	placeAnimation = true;

	glm::vec3 place;

	for (int i = 1; i < 150; i++)
	{
		place = cameraPos + placeSpeed * i * cameraFront;
		if (createWorld->mapData[round(place.x)][round(place.y)][round(place.z)] != CubeType::Null)
		{
			place = cameraPos + placeSpeed * (i - 1) * cameraFront;
			if (createWorld->mapData[round(place.x)][round(place.y)][round(place.z)] == CubeType::Null &&
				!(round(place.x) == ceil(eyePos.x - 0.5) && 
				  round(place.z) == ceil(eyePos.z - 0.5) &&
				  round(place.y) == ceil(eyePos.y - 1.5)))
			{
				createWorld->mapData[round(place.x)][round(place.y)][round(place.z)] = nowTakeCube;

				commTCP->sendPlayer({ -control->eyeRadians ,-control->bodyRadians + 90 ,control->placeAnimation ,
					control->nowTakeCube, PlaceStatus::Place,
					{ control->eyePos.x, control->eyePos.y - EYE_HEIGHT, control->eyePos.z },
					{ round(place.x), round(place.y), round(place.z)} });


				operationDB->updateBlockMap(&createWorld->mapData,
					operationDB->table_name_map.c_str(), place.x / BLOCK_WIDTH,
					place.z / BLOCK_WIDTH, place.y / BLOCK_WIDTH);


				if (createWorld->plantData[round(place.x)][round(place.y)][round(place.z)] != CubeType::Null)
				{
					operationDB->updateBlockMap(&createWorld->plantData,
						operationDB->table_name_plant.c_str(), place.x / BLOCK_WIDTH,
						place.z / BLOCK_WIDTH, place.y / BLOCK_WIDTH);
					createWorld->plantData[round(place.x)][round(place.y)][round(place.z)] = CubeType::Null;
				}
				if (createWorld->leafData[round(place.x)][round(place.y)][round(place.z)] != CubeType::Null)
				{
					operationDB->updateBlockMap(&createWorld->leafData,
						operationDB->table_name_leaf.c_str(), place.x / BLOCK_WIDTH,
						place.z / BLOCK_WIDTH, place.y / BLOCK_WIDTH);
					createWorld->leafData[round(place.x)][round(place.y)][round(place.z)] = CubeType::Null;
				}

				break;
			}
		}
	}
}

void Control::destroyBlock()
{
	placeAnimation = true;

	glm::vec3 place;

	for (int i = 1; i < 150; i++)
	{
		place = cameraPos + placeSpeed * i * cameraFront;
		if (createWorld->leafData[round(place.x)][round(place.y)][round(place.z)] != CubeType::Null)
		{
			CubeType cubeType = createWorld->leafData[round(place.x)][round(place.y)][round(place.z)];
			eliminateAnima->push({ round(place.x), round(place.y), round(place.z) }, cubeType);

			createWorld->leafData[round(place.x)][round(place.y)][round(place.z)] = CubeType::Null;


			commTCP->sendPlayer({ -control->eyeRadians ,-control->bodyRadians + 90 ,control->placeAnimation ,
				control->nowTakeCube, PlaceStatus::Eliminate,
				{ control->eyePos.x, control->eyePos.y - EYE_HEIGHT, control->eyePos.z },
				{ round(place.x), round(place.y), round(place.z)} });


			operationDB->updateBlockMap(&createWorld->leafData,
				operationDB->table_name_leaf.c_str(), place.x / BLOCK_WIDTH,
				place.z / BLOCK_WIDTH, place.y / BLOCK_WIDTH);

			return;
		}

		if (createWorld->mapData[round(place.x)][round(place.y)][round(place.z)] != CubeType::Null)
		{
			CubeType cubeType = createWorld->mapData[round(place.x)][round(place.y)][round(place.z)];
			eliminateAnima->push({ round(place.x), round(place.y), round(place.z) }, cubeType);

			createWorld->mapData[round(place.x)][round(place.y)][round(place.z)] = CubeType::Null;

			commTCP->sendPlayer({ -control->eyeRadians ,-control->bodyRadians + 90 ,control->placeAnimation ,
				control->nowTakeCube, PlaceStatus::Eliminate,
				{ control->eyePos.x, control->eyePos.y - EYE_HEIGHT, control->eyePos.z },
				{ round(place.x), round(place.y), round(place.z)} });


			operationDB->updateBlockMap(&createWorld->mapData,
				operationDB->table_name_map.c_str(), place.x / BLOCK_WIDTH,
				place.z / BLOCK_WIDTH, place.y / BLOCK_WIDTH);

			if (createWorld->plantData[round(place.x)][round(place.y) + 1][round(place.z)] != CubeType::Null)
			{
				createWorld->plantData[round(place.x)][round(place.y) + 1][round(place.z)] = CubeType::Null;

				commTCP->sendPlayer({ -control->eyeRadians ,-control->bodyRadians + 90 ,control->placeAnimation ,
					control->nowTakeCube, PlaceStatus::Eliminate,
					{ control->eyePos.x, control->eyePos.y - EYE_HEIGHT, control->eyePos.z },
					{ round(place.x), round(place.y), round(place.z)} });

				operationDB->updateBlockMap(&createWorld->plantData,
					operationDB->table_name_plant.c_str(), place.x / BLOCK_WIDTH,
					place.z / BLOCK_WIDTH, place.y / BLOCK_WIDTH);

			}

			return;
		}
	}
}

void mouseMove(GLFWwindow* window, double xpos, double ypos)
{
	if (control->isLogin)
	{
		static float lastX = Width / 2.0;
		static float lastY = Height / 2.0;

		static bool firstMouse = true;

		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		control->bodyRadians += xoffset;
		control->eyeRadians += yoffset;

		if (control->eyeRadians > 80.0f)
			control->eyeRadians = 80.0f;
		if (control->eyeRadians < -80.0f)
			control->eyeRadians = -80.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(control->bodyRadians)) * cos(glm::radians(control->eyeRadians));
		front.y = sin(glm::radians(control->eyeRadians));
		front.z = sin(glm::radians(control->bodyRadians)) * cos(glm::radians(control->eyeRadians));
		control->cameraFront = glm::normalize(front);

		if (EYE_HEIGHT == 1)
		{
			float radius = 0.25f;
			float camX = cos(glm::radians(control->bodyRadians)) * radius;
			float camZ = sin(glm::radians(control->bodyRadians)) * radius;
			control->cameraPos.x = control->eyePos.x + camX;
			control->cameraPos.z = control->eyePos.z + camZ;
		}
		else
		{
			float radius = 5.0f;
			float camX = cos(glm::radians(control->bodyRadians)) * radius;
			float camZ = sin(glm::radians(control->bodyRadians)) * radius;
			control->cameraPos.x = control->eyePos.x - camX;
			control->cameraPos.z = control->eyePos.z - camZ;
		}
	}
	else
	{
		control->moustX = xpos;
		control->moustY = ypos;
	}
}

void Control::windowKey()
{
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(glWidget->getWindow(), true);

	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
	{
		isCursorDisabled = false;
		glfwSetInputMode(glWidget->getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_C) == GLFW_PRESS)
		windowGrab();
}

void Control::cameraMoveKey()
{
	float cameraSpeed = moveSpeed * elapseTime;
	glm::vec3 oldPos = eyePos;

	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_W) == GLFW_PRESS)
		eyePos += cameraSpeed * cameraFront;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_S) == GLFW_PRESS)
		eyePos -= cameraSpeed * cameraFront;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_A) == GLFW_PRESS)
		eyePos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_D) == GLFW_PRESS)
		eyePos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

	if (eyePos != oldPos)
	{
		glm::vec3 newPos = eyePos + cameraPos - oldPos;

		if (isCollision(newPos.x, newPos.y, newPos.z))
		{
			cameraPos = newPos;
			eyePos.y = eyeAltitude;
			cameraPos.y = eyePos.y;
		}
		else
		{
			eyePos = oldPos;
		}
	}
}

void Control::cameraJumpKey()
{
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (ps == PeopleStatus::Normal)
		{
			ps = PeopleStatus::JumpUp;
			beginJumpAltitude = eyePos.y + float(EYE_HEIGHT) * 1.1;
		}
	}

	eyeHeight();
}

void Control::takeCubeKey()
{
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_1) == GLFW_PRESS)
		nowTakeCube = CubeType::MapLand;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_2) == GLFW_PRESS)
		nowTakeCube = CubeType::MapSand;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_3) == GLFW_PRESS)
		nowTakeCube = CubeType::MapStone;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_4) == GLFW_PRESS)
		nowTakeCube = CubeType::MapRedBrick;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_5) == GLFW_PRESS)
		nowTakeCube = CubeType::MapWood;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_6) == GLFW_PRESS)
		nowTakeCube = CubeType::MapAspenWood;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_7) == GLFW_PRESS)
		nowTakeCube = CubeType::MapDesertBrick;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_8) == GLFW_PRESS)
		nowTakeCube = CubeType::MapStoneBrick;
	if (glfwGetKey(glWidget->getWindow(), GLFW_KEY_9) == GLFW_PRESS)
		nowTakeCube = CubeType::MapSilverBrick;

	commTCP->sendPlayer({ -control->eyeRadians ,-control->bodyRadians + 90 ,control->placeAnimation ,
		control->nowTakeCube, PlaceStatus::Normal,
		{ control->eyePos.x, control->eyePos.y - EYE_HEIGHT, control->eyePos.z },
		{ 0, 0, 0} });
}

bool Control::isCollision(float x, float y, float z)
{
	if (x <= 10 || z <= 10 || y <= 10 || x >= WORLD_WIDTH - 10 || z >= WORLD_WIDTH - 10 || y >= WORLD_HEIGHT - 10)
		return false;

	if (createWorld->mapData[ceil(x - 0.5)][ceil(y - 1.5)][ceil(z - 0.5)] == CubeType::Null &&
		createWorld->mapData[ceil(x - 0.5)][ceil(y - 0.5)][ceil(z - 0.5)] == CubeType::Null)
	{
		if (createWorld->mapData[ceil(x - 0.5 + 0.2)][ceil(y - 1.5)][ceil(z - 0.5)] == CubeType::Null &&
			createWorld->mapData[ceil(x - 0.5 - 0.2)][ceil(y - 1.5)][ceil(z - 0.5)] == CubeType::Null &&
			createWorld->mapData[ceil(x - 0.5)][ceil(y - 1.5)][ceil(z - 0.5 + 0.2)] == CubeType::Null &&
			createWorld->mapData[ceil(x - 0.5)][ceil(y - 1.5)][ceil(z - 0.5 - 0.2)] == CubeType::Null &&

			createWorld->mapData[ceil(x - 0.5 + 0.2)][ceil(y - 0.5)][ceil(z - 0.5)] == CubeType::Null &&
			createWorld->mapData[ceil(x - 0.5 - 0.2)][ceil(y - 0.5)][ceil(z - 0.5)] == CubeType::Null &&
			createWorld->mapData[ceil(x - 0.5)][ceil(y - 0.5)][ceil(z - 0.5 + 0.2)] == CubeType::Null &&
			createWorld->mapData[ceil(x - 0.5)][ceil(y - 0.5)][ceil(z - 0.5 - 0.2)] == CubeType::Null)
			return true;
	}

	return false;
}

void Control::windowGrab()
{
	GLubyte* pPixelData;
	pPixelData = (GLubyte*)malloc(Width * Height * 4);
	if (pPixelData == 0)
		return;
	glReadBuffer(GL_FRONT);// 保存窗口渲染的结果
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);// 解压窗口数据结构
	glReadPixels(0, 0, Width, Height, GL_RGBA, GL_UNSIGNED_BYTE, pPixelData);

	cv::Mat img;
	std::vector<cv::Mat> imgPlanes;
	img.create(Height, Width, CV_8UC3);
	cv::split(img, imgPlanes);// 将图像按照通道数拆分，三个单通道序列

	for (int i = 0; i < Height; i++) 
	{
		unsigned char* plane0Ptr = imgPlanes[0].ptr<unsigned char>(i);// B
		unsigned char* plane1Ptr = imgPlanes[1].ptr<unsigned char>(i);// G
		unsigned char* plane2Ptr = imgPlanes[2].ptr<unsigned char>(i);// R
		for (int j = 0; j < Width; j++) 
		{
			int k = 4 * (i * Width + j);
			plane2Ptr[j] = pPixelData[k + 0];// R
			plane1Ptr[j] = pPixelData[k + 1];// G
			plane0Ptr[j] = pPixelData[k + 2];// B
		}
	}

	cv::merge(imgPlanes, img);// 合并多通道图像
	cv::flip(img, img, 0); // 反转图像，OpenGL 和 OpenCV 的坐标系Y轴是相反的

	static int num = 0;
	char imgName[20] = { 0 };

	sprintf(imgName, "%d.jpg", ++num);
	cv::imwrite(imgName, img);//保存图片
}