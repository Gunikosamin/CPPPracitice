#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//　ウィンドウ関連の処理
class Window
{

	//　ウィンドウのハンドル
	GLFWwindow* const window;

	//　ウィンドウのサイズ
	GLfloat size[2];

	//　ワールド座標系に対するデバイス座標系の拡大率(いくらのどっとで長さを1にするか)
	GLfloat scale; //１の長さを100画僧とするので最初は100で初期化

	//　図形の正規化デバイス座標系上での位置
	GLfloat location[2];


public:

	// コンストラクタ
	Window(int width = 640, int height = 480, const char* title = "Hello!")
		: window(glfwCreateWindow(width, height, title, NULL, NULL)), scale(100.0f), location{ 0.0f, 0.0f}
	{
		if (window == NULL)
		{
			//　ウィンドが作成できなかった
			std::cerr << "Can't create GLFW window." << std::endl;
			exit(1);
		}

		//　現在のウィンドを処理対象
		glfwMakeContextCurrent(window);

		// GLEWを初期化する
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
		   //GLEWが作成できなかった
			std::cerr << "Cant't initialize GLEW" << std::endl;
			exit(1);
		
		}

		//　垂直同期のタイミングを待つ
		glfwSwapInterval(1);


		//　ウィンドウのサイズ変更時に呼び出す処理の登録 
		glfwSetWindowSizeCallback(window, resize);

		//マウスホイール操作時に呼び出す処理の登録
		glfwSetScrollCallback(window, wheel);

		//　このインスタンスのthisポインタを記録しておく
		glfwSetWindowUserPointer(window, this);

	


		// 開いたウィンドウの初期設定
		resize(window, width, height);

	}

	//　デストラクタ
	virtual ~Window() {
		glfwDestroyWindow(window); // windowを閉じる
	}

	explicit operator bool() {
		// イベントを取り出す
		glfwWaitEvents();

		//マウスの左ボタンの状態を調べる
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE)
		{

			//　マウスカーソルの位置を取得する
			double x, y;
			glfwGetCursorPos(window, &x, &y);

			//　マウスカーソルの正規化デバイス座標系上での位置を求める
			location[0] = static_cast<GLfloat>(x) * 2.0f / size[0] - 1.0f;
			location[1] = 1.0f - static_cast<GLfloat>(y) * 2.0f / size[1];
		}

		//ウィンドウを閉じる必要がなければtrueを返す
		return !glfwWindowShouldClose(window);
	}

	//　ダブルバッファリング
	void swapBuffers() const
	{
		// カラーぶっふぁリングを入れ替える
		glfwSwapBuffers(window);
	}

	//　ウィンドウのサイズ変更時の処理
	static void resize(GLFWwindow* const window, int width, int height)
	{
	   //　フレームバッファのバッファのサイズを調べる
		int fbWidth, fbHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

		//　フレームバッファ全体をビューポートに設定する
		glViewport(0, 0, fbWidth, fbHeight);

		// このインスタンスのthisポインタを得る。Staticの関数は、メンバの変数を引き出せない。Staticの関数は自身はインスタンスを持てない
		Window* const
			instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));

		if (instance != NULL)
		{
			//　開いたウィンドウのサイズを保存する
			instance->size[0] = static_cast<GLfloat>(width);
			instance->size[1] = static_cast<GLfloat>(height);
		}

	}

	static void wheel(GLFWwindow* window, double x, double y)
	{
		// このインスタンスのthisポインタを得る
		Window* const
			instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));

		if (instance != NULL)
		{
			// ワールド座標系に対するデバイス座標系の拡大率を更新
			instance->scale += static_cast<GLfloat>(y);

		}
			

	}

	 // ウィンドウのサイズを取り出す
	const GLfloat* getSize() const { return size; }

	//ワールド座標系に対するデバイス座標系の拡大率を取り出す
	GLfloat getScale() const { return scale; }

	//　位置を取り出す
	const GLfloat* getLocation() const { return location;  }

	


};