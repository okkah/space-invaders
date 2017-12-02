#pragma once

#include <tchar.h>
#include <windows.h>
#define D3D_DEBUG_INFO
#include <stdlib.h>
#include <math.h>
#include <d3dx9.h>
#include <XAudio2.h>
#include <vector>
#include <list>

#include "../include/WindowManager.h"
#include "../include/ars.h"
#include "touchable.h"
#include "arstest.h"

#include <iostream>
#include <random>
#include <winbase.h>

//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------


void subtract_mask(Texture* result, Texture* bg, Texture* src, DWORD border);

UINT MainLoop(WindowManager *winmgr)
{
	//ShowDebugWindow();

	//for debug(1/2)
	//Window window2;
	//winmgr->RegisterWindow(&window2);

	//ARSG arsgd(window2.hWnd, sizex, sizey, true);
	//Texture debug(&arsgd, sizex, sizey);
	//debug.SetDrawMode(true);

	int i, j;
	std::random_device rnd; //非決定的な乱数生成器を生成
	std::mt19937 mt(rnd()); //メルセンヌ・ツイスタの32ビット版，引数は初期シード値
	std::uniform_int_distribution <> rand608(0, 607); //[0, 607]範囲の一様乱数
	
	Window window;
	winmgr->RegisterWindow(&window);

	ARSG g(window.hWnd, sizex, sizey + 76, true);
	g.SetBackgroundColor(255,0,0,0);

	Light light(&g);	
	g.Register(&light);

	ARSD d;
	d.Init();
	d.AttachCam(0);
	d.StartGraph();
	
	Texture hitArea(&g,sizex,sizey);
	Texture stored (&g,sizex,sizey);
	Texture source (&g,sizex,sizey);
	source.SetPosition(0, 76, 0, coorType::GL_ABSOLUTE);
	source.SetDrawMode(TRUE);
	g.Register(&source);

	Texture left(&g, L"left.png");
	left.SetPosition(360, 480-76, 0, coorType::GL_ABSOLUTE);
	left.SetDrawMode(TRUE);
	g.Register(&left);

	Texture left2(&g, L"left.png");
	left2.SetPosition(360, 480, 0, coorType::GL_ABSOLUTE);
	left2.SetDrawMode(TRUE);
	g.Register(&left2);

	Texture right(&g, L"right.png");
	right.SetPosition(520, 480-76, 0, coorType::GL_ABSOLUTE);
	right.SetDrawMode(TRUE);
	g.Register(&right);

	Texture right2(&g, L"right.png");
	right2.SetPosition(520, 480, 0, coorType::GL_ABSOLUTE);
	right2.SetDrawMode(TRUE);
	g.Register(&right2);

	Plate attack(&g, L"attack.png");
	attack.SetPosition(80, 480 - 76, 0, coorType::GL_ABSOLUTE);
	attack.SetDrawMode(TRUE);
	g.Register(&attack);

	Texture attack2(&g, L"attack.png");
	attack2.SetPosition(80, 480, 0, coorType::GL_ABSOLUTE);
	attack2.SetDrawMode(TRUE);
	g.Register(&attack2);

	Texture background(&g, L"background.jpg");
	background.SetDrawMode(TRUE);
	g.Register(&background);

	Player player(&g, L"player.png");
	player.px = 300;
	player.py = 420;
	player.SetPosition(player.px, player.py, 0, coorType::GL_ABSOLUTE);
	player.SetDrawMode(TRUE);
	g.Register(&player);

	int sflag[20];
	int sx[20], sy[20];
	int cool_time;
	Texture shot[20];
	for (i = 0; i < 20; i++)
	{
		sflag[i] = 0;
		sx[i] = 0;
		sy[i] = 0;
		shot[i].Init(&g, L"shot.png");
		shot[i].SetDrawMode(TRUE);
	}
	cool_time = 0;

	int flag[11];
	int flag1[11];
	double x1[11], y1[11];
	Texture enemy1[11];
	for (i = 0; i < 11; i++)
	{
		enemy1[i].Init(&g, L"enemy1.png");
		x1[i] = i * 55;
		y1[i] = 40;
		flag[i] = 0;
		flag1[i] = 1;
		enemy1[i].SetPosition(x1[i], y1[i], 0, coorType::GL_ABSOLUTE);
		enemy1[i].SetDrawMode(TRUE);
		g.Register(&enemy1[i]);
	}
	int flag2[11];
	double x2[11], y2[11];
	Texture enemy2[11];
	for (i = 0; i < 11; i++)
	{
		enemy2[i].Init(&g, L"enemy2.png");
		x2[i] = i * 55;
		y2[i] = 80;
		flag2[i] = 1;
		enemy2[i].SetPosition(x2[i], y2[i], 0, coorType::GL_ABSOLUTE);
		enemy2[i].SetDrawMode(TRUE);
		g.Register(&enemy2[i]);
	}
	int flag3[11];
	double x3[11], y3[11];
	Texture enemy3[11];
	for (i = 0; i < 11; i++)
	{
		enemy3[i].Init(&g, L"enemy1.png");
		x3[i] = i * 55;
		y3[i] = 120;
		flag3[i] = 1;
		enemy3[i].SetPosition(x3[i], y3[i], 0, coorType::GL_ABSOLUTE);
		enemy3[i].SetDrawMode(TRUE);
		g.Register(&enemy3[i]);
	}
	int flag4[11];
	double x4[11], y4[11];
	Texture enemy4[11];
	for (i = 0; i < 11; i++)
	{
		enemy4[i].Init(&g, L"enemy2.png");
		x4[i] = i * 55;
		y4[i] = 160;
		flag4[i] = 1;
		enemy4[i].SetPosition(x4[i], y4[i], 0, coorType::GL_ABSOLUTE);
		enemy4[i].SetDrawMode(TRUE);
		g.Register(&enemy4[i]);
	}

	int ex[20], ey[20];
	int eflag[20];
	int es_counter;
	Texture eshot[20];
	for (i = 0; i < 20; i++)
	{
		ex[i] = 0;
		ey[i] = 0;
		eflag[i] = 0;
		eshot[i].Init(&g, L"eshot.png");
		eshot[i].SetDrawMode(TRUE);
	}
	es_counter = 0;

	wchar_t*name[] = { L"blast1.png", L"blast2.png", L"blast3.png", L"blast4.png", L"blast5.png", L"blast6.png", L"blast7.png", L"blast8.png", L"blast9.png" };
	Texture blast[9];
	for (i = 0; i < 9; i++)
	{
		blast[i].Init(&g, name[i]);
		blast[i].SetDrawMode(TRUE);
	}

	Texture life(&g, L"life.png");
	life.SetPosition(440, 5, 0, coorType::GL_ABSOLUTE);
	life.SetDrawMode(TRUE);
	g.Register(&life);

	Texture life1(&g, L"player.png");
	life1.SetPosition(520, 5, 0, coorType::GL_ABSOLUTE);
	life1.SetDrawMode(TRUE);
	g.Register(&life1);

	Texture life2(&g, L"player.png");
	life2.SetPosition(560, 5, 0, coorType::GL_ABSOLUTE);
	life2.SetDrawMode(TRUE);
	g.Register(&life2);

	Texture life3(&g, L"player.png");
	life3.SetPosition(600, 5, 0, coorType::GL_ABSOLUTE);
	life3.SetDrawMode(TRUE);
	g.Register(&life3);

	int lifenumber = 3;

	Texture score(&g, L"score.png");
	score.SetPosition(5, 5, 0, coorType::GL_ABSOLUTE);
	score.SetDrawMode(TRUE);
	g.Register(&score);

	Texture s4_0(&g, L"0.png");
	s4_0.SetPosition(220, 8, 0, coorType::GL_ABSOLUTE);
	s4_0.SetDrawMode(TRUE);

	Texture s3_0(&g, L"0.png");
	s3_0.SetPosition(190, 8, 0, coorType::GL_ABSOLUTE);
	s3_0.SetDrawMode(TRUE);

	Texture s2_0(&g, L"0.png");
	s2_0.SetPosition(160, 8, 0, coorType::GL_ABSOLUTE);
	s2_0.SetDrawMode(TRUE);

	Texture s2_1(&g, L"1.png");
	s2_1.SetPosition(160, 8, 0, coorType::GL_ABSOLUTE);
	s2_1.SetDrawMode(TRUE);

	Texture s2_2(&g, L"2.png");
	s2_2.SetPosition(160, 8, 0, coorType::GL_ABSOLUTE);
	s2_2.SetDrawMode(TRUE);

	Texture s2_3(&g, L"3.png");
	s2_3.SetPosition(160, 8, 0, coorType::GL_ABSOLUTE);
	s2_3.SetDrawMode(TRUE);

	Texture s2_4(&g, L"4.png");
	s2_4.SetPosition(160, 8, 0, coorType::GL_ABSOLUTE);
	s2_4.SetDrawMode(TRUE);

	Texture s2_5(&g, L"5.png");
	s2_5.SetPosition(160, 8, 0, coorType::GL_ABSOLUTE);
	s2_5.SetDrawMode(TRUE);

	Texture s2_6(&g, L"6.png");
	s2_6.SetPosition(160, 8, 0, coorType::GL_ABSOLUTE);
	s2_6.SetDrawMode(TRUE);

	Texture s2_7(&g, L"7.png");
	s2_7.SetPosition(160, 8, 0, coorType::GL_ABSOLUTE);
	s2_7.SetDrawMode(TRUE);

	Texture s2_8(&g, L"8.png");
	s2_8.SetPosition(160, 8, 0, coorType::GL_ABSOLUTE);
	s2_8.SetDrawMode(TRUE);

	Texture s2_9(&g, L"9.png");
	s2_9.SetPosition(160, 8, 0, coorType::GL_ABSOLUTE);
	s2_9.SetDrawMode(TRUE);

	Texture s1_0(&g, L"0.png");
	s1_0.SetPosition(130, 8, 0, coorType::GL_ABSOLUTE);
	s1_0.SetDrawMode(TRUE);

	Texture s1_1(&g, L"1.png");
	s1_1.SetPosition(130, 8, 0, coorType::GL_ABSOLUTE);
	s1_1.SetDrawMode(TRUE);

	Texture s1_2(&g, L"2.png");
	s1_2.SetPosition(130, 8, 0, coorType::GL_ABSOLUTE);
	s1_2.SetDrawMode(TRUE);

	Texture s1_3(&g, L"3.png");
	s1_3.SetPosition(130, 8, 0, coorType::GL_ABSOLUTE);
	s1_3.SetDrawMode(TRUE);

	Texture s1_4(&g, L"4.png");
	s1_4.SetPosition(130, 8, 0, coorType::GL_ABSOLUTE);
	s1_4.SetDrawMode(TRUE);

	Texture s1_5(&g, L"5.png");
	s1_5.SetPosition(130, 8, 0, coorType::GL_ABSOLUTE);
	s1_5.SetDrawMode(TRUE);

	Texture s1_6(&g, L"6.png");
	s1_6.SetPosition(130, 8, 0, coorType::GL_ABSOLUTE);
	s1_6.SetDrawMode(TRUE);

	Texture s1_7(&g, L"7.png");
	s1_7.SetPosition(130, 8, 0, coorType::GL_ABSOLUTE);
	s1_7.SetDrawMode(TRUE);

	Texture s1_8(&g, L"8.png");
	s1_8.SetPosition(130, 8, 0, coorType::GL_ABSOLUTE);
	s1_8.SetDrawMode(TRUE);

	Texture s1_9(&g, L"9.png");
	s1_9.SetPosition(130, 8, 0, coorType::GL_ABSOLUTE);
	s1_9.SetDrawMode(TRUE);

	int scorenumber = 0;

	Texture title(&g, L"title.png");
	title.SetDrawMode(TRUE);

	int tflag = 0;

	Texture gameclear(&g, L"gameclear.png");
	gameclear.SetDrawMode(TRUE);

	Texture gameover(&g, L"gameover.png");
	gameover.SetDrawMode(TRUE);

	int psizex, psizey;
	int ssizex, ssizey;
	int esizex, esizey;
	int essizex, essizey;
	psizex = 30;
	psizey = 30;
	ssizex = 22;
	ssizey = 23;
	esizex = 56;
	esizey = 20;
	essizex = 20;
	essizey = 10;

	InputHandler *keyIn = window.GetInputHandler();

	while(!d.GetCamImage(&stored));

	Texture ltxtr, rtxtr, atxtr;
	int lpGx, lpGy, rpGx, rpGy, apGx, apGy;

	while (!winmgr->WaitingForTermination()){
		if (keyIn->GetKeyTrig('A'))
			d.GetCamImage(&stored);
		d.GetCamImage(&source);
		if (keyIn->GetKeyTrig('Q')) break;
			
		subtract_mask(&hitArea,&stored,&source,0x20202020);	
		
		Touchable::update(&hitArea, 100);

		//プレイヤーの移動処理
		ltxtr.Init(&g, sizex, sizey);

		unsigned int lpixel_count;

		g.Draw(&left, &ltxtr);
		ARSC::and(&ltxtr, &ltxtr, &hitArea, 0x10101010);

		ARSC::getCG(&lpGx, &lpGy, &lpixel_count, &ltxtr);

		//std::cout << lpixel_count << std::endl;

		if (lpixel_count > 500/*keyIn->GetKeyTrig('X')*/)
		{
			if (player.px > 0)
			{
				player.px = player.px - 10;
				player.SetPosition(player.px, player.py, 0, coorType::GL_ABSOLUTE);
			}
		}

		rtxtr.Init(&g, sizex, sizey);

		unsigned int rpixel_count;

		g.Draw(&right, &rtxtr);
		ARSC::and(&rtxtr, &rtxtr, &hitArea, 0x10101010);

		ARSC::getCG(&rpGx, &rpGy, &rpixel_count, &rtxtr);

		//std::cout << rpixel_count << std::endl;

		if (rpixel_count > 500/*keyIn->GetKeyTrig('C')*/)
		{
			if (player.px < 590)
			{
				player.px = player.px + 10;
				player.SetPosition(player.px, player.py, 0, coorType::GL_ABSOLUTE);
			}
		}
		

		atxtr.Init(&g, sizex, sizey);
		unsigned int apixel_count;

		g.Draw(&attack, &atxtr);
		ARSC::and(&atxtr, &atxtr, &hitArea, 0x10101010);

		ARSC::getCG(&apGx, &apGy, &apixel_count, &atxtr);

		//std::cout << apixel_count << std::endl;

		//タイトル画面
		while (tflag == 0)
		{
			g.Register(&title);
			g.Draw();

			if (keyIn->GetKeyTrig('A'))
				d.GetCamImage(&stored);
			d.GetCamImage(&source);
			if (keyIn->GetKeyTrig('Q')) break;

			subtract_mask(&hitArea, &stored, &source, 0x20202020);

			Touchable::update(&hitArea, 100);

			g.Draw(&attack, &atxtr);
			ARSC::and(&atxtr, &atxtr, &hitArea, 0x10101010);

			ARSC::getCG(&apGx, &apGy, &apixel_count, &atxtr);

			if (apixel_count > 1000 )
			{
				g.Unregister(&title);
				g.Draw();
				tflag = 1;
			}
		}
		//std::cout << attack.stflag << std::endl;

		if (apixel_count > 500/*keyIn->GetKeyTrig('Z')*/)
		{
			if (cool_time < 0 && attack.stflag == 0)
			{
				for (i = 0; i < 20; i++)
				{
					if (sflag[i] == 0)
					{
						break;
					}
				}
				if (i < 20)
				{
					sflag[i] = 1;
					attack.stflag = 1;
					sx[i] = player.px + 5;
					sy[i] = player.py;
					shot[i].SetPosition(sx[i], sy[i], 0, coorType::GL_ABSOLUTE);
					g.Register(&shot[i]);
					//std::cout << "attack" << std::endl;
					cool_time = 5;
				}
			}
		}
		cool_time -= 1;
		for (i = 0; i < 20; i++)
		{
			if (sflag[i] == 1)
			{
				sy[i] -= 20;
				shot[i].SetPosition(sx[i], sy[i], 0, coorType::GL_ABSOLUTE);
				if (sy[i] < 0)
				{
					sflag[i] = 0;
					g.Unregister(&shot[i]);
				}
			}
		}

		//敵の移動処理
		for (i = 0; i < 11; i++)
		{
			if (x1[i] > 580 && x1[i] < 700)
			{
				for (j = 0; j < 11; j++)
				{
					flag[j] = 1;
					y1[j] += 12;
					enemy1[j].SetPosition(x1[j], y1[j], 0, coorType::GL_ABSOLUTE);
					y2[j] += 12;
					enemy2[j].SetPosition(x2[j], y2[j], 0, coorType::GL_ABSOLUTE);
					y3[j] += 12;
					enemy3[j].SetPosition(x3[j], y3[j], 0, coorType::GL_ABSOLUTE);
					y4[j] += 12;
					enemy4[j].SetPosition(x4[j], y4[j], 0, coorType::GL_ABSOLUTE);
				}
			}
			else if (x2[i] > 580 && x2[i] < 700)
			{
				for (j = 0; j < 11; j++)
				{
					flag[j] = 1;
					y1[j] += 12;
					enemy1[j].SetPosition(x1[j], y1[j], 0, coorType::GL_ABSOLUTE);
					y2[j] += 12;
					enemy2[j].SetPosition(x2[j], y2[j], 0, coorType::GL_ABSOLUTE);
					y3[j] += 12;
					enemy3[j].SetPosition(x3[j], y3[j], 0, coorType::GL_ABSOLUTE);
					y4[j] += 12;
					enemy4[j].SetPosition(x4[j], y4[j], 0, coorType::GL_ABSOLUTE);
				}
			}
			else if (x3[i] > 580 && x3[i] < 700)
			{
				for (j = 0; j < 11; j++)
				{
					flag[j] = 1;
					y1[j] += 12;
					enemy1[j].SetPosition(x1[j], y1[j], 0, coorType::GL_ABSOLUTE);
					y2[j] += 12;
					enemy2[j].SetPosition(x2[j], y2[j], 0, coorType::GL_ABSOLUTE);
					y3[j] += 12;
					enemy3[j].SetPosition(x3[j], y3[j], 0, coorType::GL_ABSOLUTE);
					y4[j] += 12;
					enemy4[j].SetPosition(x4[j], y4[j], 0, coorType::GL_ABSOLUTE);
				}
			}
			else if (x4[i] > 580 && x4[i] < 700)
			{
				for (j = 0; j < 11; j++)
				{
					flag[j] = 1;
					y1[j] += 12;
					enemy1[j].SetPosition(x1[j], y1[j], 0, coorType::GL_ABSOLUTE);
					y2[j] += 12;
					enemy2[j].SetPosition(x2[j], y2[j], 0, coorType::GL_ABSOLUTE);
					y3[j] += 12;
					enemy3[j].SetPosition(x3[j], y3[j], 0, coorType::GL_ABSOLUTE);
					y4[j] += 12;
					enemy4[j].SetPosition(x4[j], y4[j], 0, coorType::GL_ABSOLUTE);
				}
			}
			else if (x1[i] < 0)
			{
				for (j = 0; j < 11; j++)
				{
					flag[j] = 0;
					y1[j] += 12;
					enemy1[j].SetPosition(x1[j], y1[j], 0, coorType::GL_ABSOLUTE);
					y2[j] += 12;
					enemy2[j].SetPosition(x2[j], y2[j], 0, coorType::GL_ABSOLUTE);
					y3[j] += 12;
					enemy3[j].SetPosition(x3[j], y3[j], 0, coorType::GL_ABSOLUTE);
					y4[j] += 12;
					enemy4[j].SetPosition(x4[j], y4[j], 0, coorType::GL_ABSOLUTE);
				}
			}
			else if (x2[i] < 0)
			{
				for (j = 0; j < 11; j++)
				{
					flag[j] = 0;
					y1[j] += 12;
					enemy1[j].SetPosition(x1[j], y1[j], 0, coorType::GL_ABSOLUTE);
					y2[j] += 12;
					enemy2[j].SetPosition(x2[j], y2[j], 0, coorType::GL_ABSOLUTE);
					y3[j] += 12;
					enemy3[j].SetPosition(x3[j], y3[j], 0, coorType::GL_ABSOLUTE);
					y4[j] += 12;
					enemy4[j].SetPosition(x4[j], y4[j], 0, coorType::GL_ABSOLUTE);
				}
			}
			else if (x3[i] < 0)
			{
				for (j = 0; j < 11; j++)
				{
					flag[j] = 0;
					y1[j] += 12;
					enemy1[j].SetPosition(x1[j], y1[j], 0, coorType::GL_ABSOLUTE);
					y2[j] += 12;
					enemy2[j].SetPosition(x2[j], y2[j], 0, coorType::GL_ABSOLUTE);
					y3[j] += 12;
					enemy3[j].SetPosition(x3[j], y3[j], 0, coorType::GL_ABSOLUTE);
					y4[j] += 12;
					enemy4[j].SetPosition(x4[j], y4[j], 0, coorType::GL_ABSOLUTE);
				}
			}
			else if (x4[i] < 0)
			{
				for (j = 0; j < 11; j++)
				{
					flag[j] = 0;
					y1[j] += 12;
					enemy1[j].SetPosition(x1[j], y1[j], 0, coorType::GL_ABSOLUTE);
					y2[j] += 12;
					enemy2[j].SetPosition(x2[j], y2[j], 0, coorType::GL_ABSOLUTE);
					y3[j] += 12;
					enemy3[j].SetPosition(x3[j], y3[j], 0, coorType::GL_ABSOLUTE);
					y4[j] += 12;
					enemy4[j].SetPosition(x4[j], y4[j], 0, coorType::GL_ABSOLUTE);
				}
			}
			if (flag[i] == 0)
			{
				for (j = 0; j < 11; j++)
				{
					x1[j] += 0.05;
					enemy1[j].SetPosition(x1[j], y1[j], 0, coorType::GL_ABSOLUTE);
					x2[j] += 0.05;
					enemy2[j].SetPosition(x2[j], y2[j], 0, coorType::GL_ABSOLUTE);
					x3[j] += 0.05;
					enemy3[j].SetPosition(x3[j], y3[j], 0, coorType::GL_ABSOLUTE);
					x4[j] += 0.05;
					enemy4[j].SetPosition(x4[j], y4[j], 0, coorType::GL_ABSOLUTE);
				}
			}
			else if (flag[i] == 1)
			{
				for (j = 0; j < 11; j++)
				{
					x1[j] -= 0.05;
					enemy1[j].SetPosition(x1[j], y1[j], 0, coorType::GL_ABSOLUTE);
					x2[j] -= 0.05;
					enemy2[j].SetPosition(x2[j], y2[j], 0, coorType::GL_ABSOLUTE);
					x3[j] -= 0.05;
					enemy3[j].SetPosition(x3[j], y3[j], 0, coorType::GL_ABSOLUTE);
					x4[j] -= 0.05;
					enemy4[j].SetPosition(x4[j], y4[j], 0, coorType::GL_ABSOLUTE);
				}
			}
		}

		if (es_counter % 7 == 0)
		{
			for (i = 0; i < 20; i++)
			{
				if (eflag[i] == 0)
				{
					break;
				}
			}
			if (i < 20)
			{
				eflag[i] = 1;
				ex[i] = rand608(mt);
				ey[i] = y4[0] + 70;
				eshot[i].SetPosition(ex[i], ey[i], 0, coorType::GL_ABSOLUTE);
				g.Register(&eshot[i]);
			}
		}
		es_counter += 1;
		for (i = 0; i < 20; i++)
		{
			if (eflag[i] == 1)
			{
				ey[i] += 7;
				eshot[i].SetPosition(ex[i], ey[i], 0, coorType::GL_ABSOLUTE);
				if (ey[i] > 440)
				{
					eflag[i] = 0;
					g.Unregister(&eshot[i]);
					ey[i] = 1000;
				}
			}
		}

		//自分と敵の当たり判定
		for (i = 0; i < 20; i++)
		{
			if (ex[i] < player.px + psizex && ex[i] + essizex > player.px && ey[i] < player.py + psizey && ey[i] + essizey > player.py)
			{
				if (lifenumber == 3)
				{
					g.Unregister(&life3);
					ey[i] = 1000;
					for (j = 0; j < 20; j++)
					{
						ey[j] = 1000;
					}

					g.Unregister(&player);
					for (i = 0; i < 9; i++)
					{
						blast[i].SetPosition(player.px, player.py, 0, coorType::GL_ABSOLUTE);
						g.Register(&blast[i]);
						g.Draw();
						g.Unregister(&blast[i]);
						g.Draw();
					}
					Sleep(1000);

					player.px = 300;
					player.py = 420;
					player.SetPosition(player.px, player.py, 0, coorType::GL_ABSOLUTE);
					g.Register(&player);
					lifenumber = 2;
				}
				else if (lifenumber == 2)
				{
					g.Unregister(&life2);
					ey[i] = 1000;
					for (j = 0; j < 20; j++)
					{
						ey[j] = 1000;
					}

					g.Unregister(&player);
					for (i = 0; i < 9; i++)
					{
						blast[i].SetPosition(player.px, player.py, 0, coorType::GL_ABSOLUTE);
						g.Register(&blast[i]);
						g.Draw();
						g.Unregister(&blast[i]);
						g.Draw();
					}
					Sleep(1000);

					player.px = 300;
					player.py = 420;
					player.SetPosition(player.px, player.py, 0, coorType::GL_ABSOLUTE);
					g.Register(&player);
					lifenumber = 1;
				}
				else if (lifenumber == 1)
				{
					g.Unregister(&life1);
					ey[i] = 1000;
					for (j = 0; j < 20; j++)
					{
						ey[j] = 1000;
					}

					g.Unregister(&player);
					for (i = 0; i < 9; i++)
					{
						blast[i].SetPosition(player.px, player.py, 0, coorType::GL_ABSOLUTE);
						g.Register(&blast[i]);
						g.Draw();
						g.Unregister(&blast[i]);
						g.Draw();
					}
					Sleep(1000);

					player.px = 300;
					player.py = 420;
					player.SetPosition(player.px, player.py, 0, coorType::GL_ABSOLUTE);
					g.Register(&player);
					lifenumber = 0;
				}
				else if (lifenumber == 0)
				{
					ey[i] = 1000;
					g.Unregister(&eshot[i]);

					g.Unregister(&player);
					for (i = 0; i < 9; i++)
					{
						blast[i].SetPosition(player.px, player.py, 0, coorType::GL_ABSOLUTE);
						g.Register(&blast[i]);
						g.Draw();
						g.Unregister(&blast[i]);
						g.Draw();
					}
					Sleep(1000);

					g.Register(&gameover);
					g.Draw();
					Sleep(10000000000);
				}
			}
		}

		//弾と敵の当たり判定
		for (i = 0; i < 20; i++)
		{
			for (j = 0; j < 11; j++)
			{
				if (flag1[j] == 1)
				{
					if (sx[i] < x1[j] + esizex && sx[i] + ssizex > x1[j] && sy[i] < y1[j] + esizey && sy[i] + ssizey > y1[j])
					{
						g.Unregister(&shot[i]);
						sflag[i] = 0;
						sy[i] = 1000;
						g.Unregister(&enemy1[j]);
						flag1[j] = 0;
						x1[j] = 10000;
						scorenumber += 100;
						//std::cout << scorenumber << std::endl;
					}
				}
			}
		}
		for (i = 0; i < 20; i++)
		{
			for (j = 0; j < 11; j++)
			{
				if (flag2[j] == 1)
				{
					if (sx[i] < x2[j] + esizex && sx[i] + ssizex > x2[j] && sy[i] < y2[j] + esizey && sy[i] + ssizey > y2[j])
					{
						g.Unregister(&shot[i]);
						sflag[i] = 0;
						sy[i] = 1000;
						g.Unregister(&enemy2[j]);
						flag2[j] = 0;
						x2[j] = 10000;
						scorenumber += 100;
						//std::cout << scorenumber << std::endl;
					}
				}
			}
		}
		for (i = 0; i < 20; i++)
		{
			for (j = 0; j < 11; j++)
			{
				if (flag3[j] == 1)
				{
					if (sx[i] < x3[j] + esizex && sx[i] + ssizex > x3[j] && sy[i] < y3[j] + esizey && sy[i] + ssizey > y3[j])
					{
						g.Unregister(&shot[i]);
						sflag[i] = 0;
						sy[i] = 1000;
						g.Unregister(&enemy3[j]);
						flag3[j] = 0;
						x3[j] = 10000;
						scorenumber += 100;
						//std::cout << scorenumber << std::endl;
					}
				}
			}
		}
		for (i = 0; i < 20; i++)
		{
			for (j = 0; j < 11; j++)
			{
				if (flag4[j] == 1)
				{
					if (sx[i] < x4[j] + esizex && sx[i] + ssizex > x4[j] && sy[i] < y4[j] + esizey && sy[i] + ssizey > y4[j])
					{
						g.Unregister(&shot[i]);
						sflag[i] = 0;
						sy[i] = 1000;
						g.Unregister(&enemy4[j]);
						flag4[j] = 0;
						x4[j] = 10000;
						scorenumber += 100;
						//std::cout << scorenumber << std::endl;
					}
				}
			}
		}

		//Score
		if (scorenumber == 0)
		{
			
			g.Register(&s1_0);
		}
		else if (scorenumber == 100)
		{
			g.Unregister(&s1_0);
			g.Register(&s1_1);
			g.Register(&s2_0);
			g.Register(&s3_0);
		}
		else if (scorenumber == 200)
		{
			g.Unregister(&s1_0);
			g.Unregister(&s1_1);
			g.Register(&s1_2);
		}
		else if (scorenumber == 300)
		{
			g.Unregister(&s1_1);
			g.Unregister(&s1_2);
			g.Register(&s1_3);
		}
		else if (scorenumber == 400)
		{
			g.Unregister(&s1_2);
			g.Unregister(&s1_3);
			g.Register(&s1_4);
		}
		else if (scorenumber == 500)
		{
			g.Unregister(&s1_3);
			g.Unregister(&s1_4);
			g.Register(&s1_5);
		}
		else if (scorenumber == 600)
		{
			g.Unregister(&s1_4);
			g.Unregister(&s1_5);
			g.Register(&s1_6);
		}
		else if (scorenumber == 700)
		{
			g.Unregister(&s1_5);
			g.Unregister(&s1_6);
			g.Register(&s1_7);
		}
		else if (scorenumber == 800)
		{
			g.Unregister(&s1_6);
			g.Unregister(&s1_7);
			g.Register(&s1_8);
		}
		else if (scorenumber == 900)
		{
			g.Unregister(&s1_7);
			g.Unregister(&s1_8);
			g.Register(&s1_9);
		}
		else if (scorenumber == 1000)
		{
			g.Unregister(&s1_8);
			g.Unregister(&s1_9);
			g.Register(&s1_1);
			g.Register(&s4_0);
		}
		else if (scorenumber == 1100)
		{
			g.Unregister(&s1_9);
			g.Unregister(&s1_1);
			g.Unregister(&s4_0);
			g.Unregister(&s2_0);
			g.Register(&s1_1);
			g.Register(&s2_1);
			g.Register(&s4_0);
		}
		else if (scorenumber == 1200)
		{
			g.Unregister(&s2_0);
			g.Unregister(&s2_1);
			g.Register(&s2_2);
		}
		else if (scorenumber == 1300)
		{
			g.Unregister(&s2_1);
			g.Unregister(&s2_2);
			g.Register(&s2_3);
		}
		else if (scorenumber == 1400)
		{
			g.Unregister(&s2_2);
			g.Unregister(&s2_3);
			g.Register(&s2_4);
		}
		else if (scorenumber == 1500)
		{
			g.Unregister(&s2_3);
			g.Unregister(&s2_4);
			g.Register(&s2_5);
		}
		else if (scorenumber == 1600)
		{
			g.Unregister(&s2_4);
			g.Unregister(&s2_5);
			g.Register(&s2_6);
		}
		else if (scorenumber == 1700)
		{
			g.Unregister(&s2_5);
			g.Unregister(&s2_6);
			g.Register(&s2_7);
		}
		else if (scorenumber == 1800)
		{
			g.Unregister(&s2_6);
			g.Unregister(&s2_7);
			g.Register(&s2_8);
		}
		else if (scorenumber == 1900)
		{
			g.Unregister(&s2_7);
			g.Unregister(&s2_8);
			g.Register(&s2_9);
		}
		else if (scorenumber == 2000)
		{
			g.Unregister(&s2_8);
			g.Unregister(&s2_9);
			g.Unregister(&s1_1);
			g.Register(&s1_2);
			g.Register(&s2_0);
		}
		else if (scorenumber == 2100)
		{
			g.Unregister(&s2_9);
			g.Unregister(&s2_0);
			g.Unregister(&s1_1);
			g.Unregister(&s1_2);
			g.Register(&s1_2);
			g.Register(&s2_1);
		}
		else if (scorenumber == 2200)
		{
			g.Unregister(&s2_0);
			g.Unregister(&s2_1);
			g.Register(&s2_2);
		}
		else if (scorenumber == 2300)
		{
			g.Unregister(&s2_1);
			g.Unregister(&s2_2);
			g.Register(&s2_3);
		}
		else if (scorenumber == 2400)
		{
			g.Unregister(&s2_2);
			g.Unregister(&s2_3);
			g.Register(&s2_4);
		}
		else if (scorenumber == 2500)
		{
			g.Unregister(&s2_3);
			g.Unregister(&s2_4);
			g.Register(&s2_5);
		}
		else if (scorenumber == 2600)
		{
			g.Unregister(&s2_4);
			g.Unregister(&s2_5);
			g.Register(&s2_6);
		}
		else if (scorenumber == 2700)
		{
			g.Unregister(&s2_5);
			g.Unregister(&s2_6);
			g.Register(&s2_7);
		}
		else if (scorenumber == 2800)
		{
			g.Unregister(&s2_6);
			g.Unregister(&s2_7);
			g.Register(&s2_8);
		}
		else if (scorenumber == 2900)
		{
			g.Unregister(&s2_7);
			g.Unregister(&s2_8);
			g.Register(&s2_9);
		}
		else if (scorenumber == 3000)
		{
			g.Unregister(&s2_8);
			g.Unregister(&s2_9);
			g.Unregister(&s1_2);
			g.Register(&s1_3);
			g.Register(&s2_0);
		}
		else if (scorenumber == 3100)
		{
			g.Unregister(&s2_9);
			g.Unregister(&s2_0);
			g.Unregister(&s1_2);
			g.Unregister(&s1_3);
			g.Register(&s1_3);
			g.Register(&s2_1);
		}
		else if (scorenumber == 3200)
		{
			g.Unregister(&s2_0);
			g.Unregister(&s2_1);
			g.Register(&s2_2);
		}
		else if (scorenumber == 3300)
		{
			g.Unregister(&s2_1);
			g.Unregister(&s2_2);
			g.Register(&s2_3);
		}
		else if (scorenumber == 3400)
		{
			g.Unregister(&s2_2);
			g.Unregister(&s2_3);
			g.Register(&s2_4);
		}
		else if (scorenumber == 3500)
		{
			g.Unregister(&s2_3);
			g.Unregister(&s2_4);
			g.Register(&s2_5);
		}
		else if (scorenumber == 3600)
		{
			g.Unregister(&s2_4);
			g.Unregister(&s2_5);
			g.Register(&s2_6);
		}
		else if (scorenumber == 3700)
		{
			g.Unregister(&s2_5);
			g.Unregister(&s2_6);
			g.Register(&s2_7);
		}
		else if (scorenumber == 3800)
		{
			g.Unregister(&s2_6);
			g.Unregister(&s2_7);
			g.Register(&s2_8);
		}
		else if (scorenumber == 3900)
		{
			g.Unregister(&s2_7);
			g.Unregister(&s2_8);
			g.Register(&s2_9);
		}
		else if (scorenumber == 4000)
		{
			g.Unregister(&s2_8);
			g.Unregister(&s2_9);
			g.Unregister(&s1_3);
			g.Register(&s1_4);
			g.Register(&s2_0);
		}
		else if (scorenumber == 4100)
		{
			g.Unregister(&s2_9);
			g.Unregister(&s2_0);
			g.Unregister(&s1_3);
			g.Unregister(&s1_4);
			g.Register(&s1_4);
			g.Register(&s2_1);
		}
		else if (scorenumber == 4200)
		{
			g.Unregister(&s2_0);
			g.Unregister(&s2_1);
			g.Register(&s2_2);
		}
		else if (scorenumber == 4300)
		{
			g.Unregister(&s2_1);
			g.Unregister(&s2_2);
			g.Register(&s2_3);
		}
		else if (scorenumber == 4400)
		{
			g.Unregister(&s2_2);
			g.Unregister(&s2_3);
			g.Register(&s2_4);
			Sleep(1000);
			g.Register(&gameclear);
			g.Draw();
			Sleep(10000000000);
		}

		//for debug(2/2)
		//debug = hitArea;
		//arsgd.Draw(&debug);
		g.Draw();
	}
	d.StopGraph();
	return 0;
}

inline void subtract_mask(Texture* result, Texture* backgrnd, Texture* src, DWORD border)
{
	ARSC::diff(result,backgrnd,src,border);
	ARSC::monochrome(result,result);
	ARSC::thresholding(result,result,border);
}

inline void Plate::onTouch(Event* e)
{
	
}

inline void Plate::onLeave()
{
	stflag = 0;
}


int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	WindowManager program(hInstance, &MainLoop);
#ifdef DEBUG
    MessageBox(NULL,L"OK?",TEXT(APPNAME), NULL);
#endif
    return 0;
}