
# include <Siv3D.hpp> // OpenSiv3D v0.4.3
# include"Board.h"

using App = SceneManager<String>;

class Title : public App::Scene
{
private:
	RectF PlayButton;
	RectF HelpButton;
	RectF RankingButton;
	const Audio bgm;

public:
	Title(const InitData& init)
		: IScene{ init }
		, PlayButton(Arg::center(Scene::Width()/2, Scene::Height()/2+200), 400, 90)
		, HelpButton(Arg::center(Scene::Width() / 2, Scene::Height() / 2 + 300), 400, 90)
		, RankingButton(Arg::center(Scene::Width() / 2, Scene::Height() / 2 + 400), 400, 90)
		, bgm{ U"./sound/Title.mp3", Arg::loop = true }
	{
		bgm.play();
	}

	void update() override
	{
		if (PlayButton.leftClicked())
		{
			changeScene(U"Game");
		}
		if (HelpButton.leftClicked())
		{
			changeScene(U"Help");
		}
		if (RankingButton.leftClicked())
		{
			changeScene(U"Ranking");
		}
	}

	void draw() const override
	{
		Scene::SetBackground(ColorF(0.2, 0.8, 0.6));

		FontAsset(U"TitleFont")(U"Twonake").drawAt(Scene::Width() / 2, Scene::Height() / 2 - 200);

		{
			PlayButton.draw(ColorF(Palette::Midnightblue));
			FontAsset(U"MenuFont")(U"はじめる").drawAt(Scene::Width() / 2, Scene::Height() / 2 + 200);
		}

		{
			HelpButton.draw(ColorF(Palette::Midnightblue));
			FontAsset(U"MenuFont")(U"遊び方").drawAt(Scene::Width() / 2, Scene::Height() / 2 + 300);
		}

		{
			RankingButton.draw(ColorF(Palette::Midnightblue));
			FontAsset(U"MenuFont")(U"ランキング").drawAt(Scene::Width() / 2, Scene::Height() / 2 + 400);
		}
	}
};

class Help : public App::Scene
{
private:
	const int32 distanceFieldSize;
	const SDFFont helpFont;
	const Texture textureHelp;
	const Audio bgm;

public:
	Help(const InitData& init)
		: IScene{init}
		, distanceFieldSize{ 60 }
		, helpFont{ distanceFieldSize, Typeface::Default }
		, textureHelp(U"./image/Help.png")
		, bgm(U"./sound/Title.mp3", Arg::loop=true)
	{
		bgm.play();
	}

	void update() override
	{
		if (KeySpace.down())
		{
			changeScene(U"Title");
		}
	}

	// ヘルプ画面キャプチャ用。処理が重たいのでリリース時には使わない。
	void drawHelp() const
	{
		Graphics2D::SetSDFParameters(helpFont.pixelRange(), 0.0);
		helpFont(U"みどり(もようの無い方)の操作方法：Aで左回転、Dで右回転").draw(40, Vec2(40, 40), Palette::Black);
		helpFont(U"あか(もようのある方)の操作方法：Jかマウス左クリックで左回転、Lかマウス右クリックで右回転").draw(40, Vec2(40, 120), Palette::Black);
		helpFont(U"スコアの計算方法：みどりのスコア×あかのスコア").draw(40, Vec2(40, 200), Palette::Black);
		helpFont(U"遊び方：").draw(40, Vec2(40, 280), Palette::Black);
		helpFont(U"2匹のヘビを操作してご飯を食べさせてあげよう！").draw(40, Vec2(200, 280), Palette::Black);
		helpFont(U"みどりのヘビは青りんご、あかのヘビはさくらんぼしか食べられないよ！").draw(40, Vec2(200, 360), Palette::Black);
		helpFont(U"あたまがかべや体に当たったり、食べられない食べ物に当たるとゲームオーバー！").draw(40, Vec2(200, 440), Palette::Black);

		helpFont(U"制作：マイコン研究会 済木").draw(40, Vec2(40, 600), Palette::Black);
		helpFont(U"音楽：魔王魂").draw(40, Vec2(40, 680), Palette::Black);
		helpFont(U"Tools：OpenSiv3D").draw(40, Vec2(40, 760), Palette::Black);

		helpFont(U"スペースキーでタイトルにもどる").drawAt(40, Vec2(Scene::Width() / 2, Scene::Height() - 200), Palette::Black);
	}

	void drawHelpImage() const
	{
		textureHelp.draw(0, 0);
	}

	void draw() const override
	{
		//drawHelp();
		drawHelpImage();
	}
};

class Ranking : public App::Scene
{
private:
	const int32 distanceFieldSize;
	const SDFFont scoreFont;
	const SDFFont titleBackFont;
	Array<int> scores;
	const Texture rankingTexture;
	const Audio bgm;

public:
	Ranking(const InitData& init)
		: IScene{ init }
		, distanceFieldSize{ 60 }
		, scoreFont{ distanceFieldSize, Typeface::Heavy }
		, titleBackFont{distanceFieldSize, Typeface::Default}
		, rankingTexture{U"./image/Ranking.png"}
		, bgm{U"./sound/Title.mp3", Arg::loop=true}
	{
		TextReader reader(U"./score.txt");

		// オープンに失敗
		if (!reader)
		{
			throw Error(U"Can't read `socre.txt`");
		}

		// 行の内容を読み込む変数
		String line;

		// 終端に達するまで 1 行ずつ読み込む
		while (reader.readLine(line))
		{
			scores.push_back(Parse<int32>(line));
		}

		reader.close();

		bgm.play();
	}



	void update() override
	{
		if (KeySpace.down())
		{
			changeScene(U"Title");
		}
	}

	void drawBackgroundImg() const
	{
		rankingTexture.draw(0, 0);
	}

	void drawBackground() const
	{
		int yInterval = 120;

		Graphics2D::SetSDFParameters(scoreFont.pixelRange(), 0.2);
		scoreFont(U"1位：").draw(80, Vec2(40, 40), Palette::Black);
		scoreFont(U"2位：").draw(80, Vec2(40, 40 + yInterval * 1), Palette::Black);
		scoreFont(U"3位：").draw(80, Vec2(40, 40 + yInterval * 2), Palette::Black);
		scoreFont(U"4位：").draw(80, Vec2(40, 40 + yInterval * 3), Palette::Black);

		Graphics2D::SetSDFParameters(scoreFont.pixelRange(), 0.0);
		scoreFont(U"1位：").draw(80, Vec2(40, 40), Palette::Gold);
		scoreFont(U"2位：").draw(80, Vec2(40, 40 + yInterval * 1), Palette::Silver);
		scoreFont(U"3位：").draw(80, Vec2(40, 40 + yInterval * 2), Palette::Chocolate);
		scoreFont(U"4位：").draw(80, Vec2(40, 40 + yInterval * 3), Palette::Skyblue);

		Graphics2D::SetSDFParameters(scoreFont.pixelRange(), 0.0);
		titleBackFont(U"スペースキーでタイトルにもどる").drawAt(40, Vec2(Scene::Width() / 2, Scene::Height() - 200), Palette::Black);
	}

	void draw() const override
	{
		drawBackgroundImg();
		//drawBackground();

		int yInterval = 120;

		FontAsset(U"RankScoreFont")(scores[0]).draw(Vec2(240, 40), Palette::Black);
		FontAsset(U"RankScoreFont")(scores[1]).draw(Vec2(240, 40 + yInterval * 1), Palette::Black);
		FontAsset(U"RankScoreFont")(scores[2]).draw(Vec2(240, 40 + yInterval * 2), Palette::Black);
		FontAsset(U"RankScoreFont")(scores[3]).draw(Vec2(240, 40 + yInterval * 3), Palette::Black);
	}
};

class Game : public App::Scene
{
private:
	const int boardTextureX, boardTextureY;
	const int boardX, boardY;
	Texture m_texture;
	const RenderTexture boardTexture;
	Board board;
	bool canStep;
	Stopwatch stopwatch;
	int stepInterval;
	bool isGameover;
	const bool IS_GREEN;
	const bool IS_RED;
	const int32 distanceFieldSize;
	const SDFFont scoreFont;
	bool wasScoreWrited;
	int nextGreen, nextRed;
	const int inputInterval;
	const Texture greenFood, redFood;
	const Audio gameBgm;
	const Audio gameoverBgm;

public:
	Game(const InitData& init)
		: IScene{ init }
		, boardTextureX{ 800 }
		, boardTextureY{ 800 }
		, boardTexture{ boardTextureX, boardTextureY, Palette::White }
		, boardX{ 20 }
		, boardY{ 20 }
		, board(boardX, boardY)
		, canStep(true)
		, stopwatch(false)
		, stepInterval(300)
		, isGameover(false)
		, IS_GREEN(true)
		, IS_RED(false)
		, distanceFieldSize(60)
		, scoreFont(distanceFieldSize, Typeface::Bold)
		, wasScoreWrited{ false }
		, nextGreen{ Straight }
		, nextRed{ Straight }
		, inputInterval{200}
		, greenFood(Emoji(U"🍏"))
		, redFood(Emoji(U"🍒"))
		, gameBgm{U"./sound/Game.mp3", Arg::loop=true}
		, gameoverBgm{U"./sound/Gameover.mp3", Arg::loop=true}
	{
		gameBgm.play();
		gameoverBgm.stop();
	}

	void writeScore(int score)
	{
		TextReader reader(U"./score.txt");

		// オープンに失敗
		if (!reader)
		{
			throw Error(U"Can't read `socre.txt`");
		}

		// 行の内容を読み込む変数
		String line;

		Array<int32> scores;

		// 終端に達するまで 1 行ずつ読み込む
		while (reader.readLine(line))
		{
			scores.push_back(Parse<int32>(line));
		}

		scores.push_back(score);
		scores.rsort();
		scores.pop_back();

		reader.close();


		// スコアの書き込み
		TextWriter writer(U"./score.txt");

		// オープンに失敗
		if (!writer)
		{
			throw Error(U"Can't write `socre.txt`");
		}

		for (int i = 0; i < scores.size(); i++)
		{
			writer.writeln(scores[i]);
		}

		writer.close();
	}

	void update() override
	{
		if (isGameover)
		{
			const bool wasBgmChanged = wasScoreWrited;

			if (!wasScoreWrited)
			{
				int score = board.getGreenScore() * board.getRedScore();
				writeScore(score);

				wasScoreWrited = true;
			}

			if (!wasBgmChanged)
			{
				gameBgm.stop();
				gameoverBgm.play();
			}

			if (KeySpace.pressed())
			{
				changeScene(U"Title");
			}

			return;
		}

		
		if (KeyA.down())
		{
			nextGreen = Left;
		}
		if (KeyD.down())
		{
			nextGreen = Right;
		}
		if (KeyJ.down() || MouseL.down())
		{
			nextRed = Left;
		}
		if (KeyL.down() || MouseR.down())
		{
			nextRed = Right;
		}

		board.setNextDirection(nextGreen, nextRed);

		if (stopwatch.ms() > stepInterval)
		{
			canStep = true;
		}

		if (canStep)
		{

			canStep = false;

			stopwatch.restart();

			isGameover = board.nextStep();

			nextGreen = Straight;
			nextRed = Straight;
		}

	}

	void draw() const override
	{

		Scene::SetBackground(ColorF(0.2, 0.8, 0.6));

		boardTexture.clear(Palette::White);

		{
			const ScopedRenderTarget2D target{ boardTexture };

			int gridInterval = boardTexture.width() / boardX;


			for (int x = 1; x < board.xSize; x++)
			{
				Line(x * gridInterval, 0, x * gridInterval, boardTexture.height()).draw(8, ColorF(Palette::Gray));
			}
			for (int y = 1; y < board.ySize; y++)
			{
				Line(0, y * gridInterval, boardTexture.width(), y * gridInterval).draw(8, ColorF(Palette::Gray));
			}

			Line(0, 0, boardTexture.width(), 0).draw(8, ColorF(Palette::Black));
			Line(0, 0, 0, boardTexture.height()).draw(8, ColorF(Palette::Black));
			Line(boardTexture.width(), 0, boardTexture.width(), boardTexture.height()).draw(8, ColorF(Palette::Black));
			Line(0, boardTexture.height(), boardTexture.width(), boardTexture.height()).draw(8, ColorF(Palette::Black));
		}

		{
			const ScopedRenderTarget2D target{ boardTexture };

			int gridInterval = boardTexture.width() / boardX;

			std::pair<int, int> greenItem = board.getGreenItem();
			std::pair<int, int> redItem = board.getRedItem();

			greenFood.resized(Vec2(gridInterval, gridInterval)).draw(greenItem.first * gridInterval, greenItem.second * gridInterval);
			redFood.resized(Vec2(gridInterval, gridInterval)).draw(redItem.first * gridInterval, redItem.second * gridInterval);
		}

		{
			const ScopedRenderTarget2D target{ boardTexture };

			int gridInterval = boardTexture.width() / boardX;

			for (int i = 0; i < board.getSnakeLen(IS_GREEN); i++)
			{
				std::pair<int, int> piece = board.getSnakePiece(i, IS_GREEN);

				Rect(piece.first * gridInterval, piece.second * gridInterval,gridInterval, gridInterval).draw(ColorF(Palette::Green));
			}

			std::pair<int, int> head = board.getSnakePiece(board.getSnakeLen(IS_GREEN) - 1, IS_GREEN);
			int direction = board.getSnakeDirection(IS_GREEN);

			std::pair<int, int> leftEye, rightEye;

			leftEye = getEyePos(false, gridInterval, direction);
			rightEye = getEyePos(true, gridInterval, direction);

			Circle(leftEye.first + head.first * gridInterval, leftEye.second + head.second * gridInterval, gridInterval / 5.0).draw(ColorF(Palette::White));
			Circle(rightEye.first + head.first * gridInterval, rightEye.second + head.second * gridInterval, gridInterval / 5.0).draw(ColorF(Palette::White));
			Circle(leftEye.first + head.first * gridInterval, leftEye.second + head.second * gridInterval, gridInterval / 7.0).draw(ColorF(Palette::Black));
			Circle(rightEye.first + head.first * gridInterval, rightEye.second + head.second * gridInterval, gridInterval / 7.0).draw(ColorF(Palette::Black));
		}

		{
			const ScopedRenderTarget2D target{ boardTexture };

			int gridInterval = boardTexture.width() / boardX;

			for (int i = 0; i < board.getSnakeLen(IS_RED); i++)
			{
				std::pair<int, int> piece = board.getSnakePiece(i, IS_RED);

				Rect(piece.first * gridInterval, piece.second * gridInterval, gridInterval, gridInterval).draw(ColorF(Palette::Red));
				Shape2D::Cross(gridInterval / 2.0, 3.0, Vec2(piece.first * gridInterval + gridInterval / 2.0, piece.second * gridInterval + gridInterval / 2.0)).draw(ColorF(Palette::Purple));
			}

			std::pair<int, int> head = board.getSnakePiece(board.getSnakeLen(IS_RED) - 1, IS_RED);
			int direction = board.getSnakeDirection(IS_RED);

			std::pair<int, int> leftEye, rightEye;

			leftEye = getEyePos(false, gridInterval, direction);
			rightEye = getEyePos(true, gridInterval, direction);

			Circle(leftEye.first + head.first * gridInterval, leftEye.second + head.second * gridInterval, gridInterval / 5.0).draw(ColorF(Palette::White));
			Circle(rightEye.first + head.first * gridInterval, rightEye.second + head.second * gridInterval, gridInterval / 5.0).draw(ColorF(Palette::White));
			Circle(leftEye.first + head.first * gridInterval, leftEye.second + head.second * gridInterval, gridInterval / 7.0).draw(ColorF(Palette::Black));
			Circle(rightEye.first + head.first * gridInterval, rightEye.second + head.second * gridInterval, gridInterval / 7.0).draw(ColorF(Palette::Black));

		}

		boardTexture.draw(Scene::Width()/2-boardTexture.width()/2, Scene::Height()/2-boardTexture.height()/2);

		// draw scores
		{
			int greenScore = board.getGreenScore();
			int redScore = board.getRedScore();

			Graphics2D::SetSDFParameters(scoreFont.pixelRange(),0.2);
			scoreFont(U"GREEN").drawAt(80, Vec2(140, 40), Palette::Black);
			scoreFont(U"RED").drawAt(80, Vec2(Scene::Width() - 140, 40), Palette::Black);
			scoreFont(greenScore).drawAt(80, Vec2(140, 120), Palette::Black);
			scoreFont(redScore).drawAt(80, Vec2(Scene::Width() - 140, 120), Palette::Black);
			
			Graphics2D::SetSDFParameters(scoreFont.pixelRange(), 0.0);
			scoreFont(U"GREEN").drawAt(80, Vec2(140, 40), Palette::Green);
			scoreFont(U"RED").drawAt(80, Vec2(Scene::Width() - 140, 40), Palette::Red);
			scoreFont(greenScore).drawAt(80, Vec2(140, 120), Palette::Green);
			scoreFont(redScore).drawAt(80, Vec2(Scene::Width() - 140, 120), Palette::Red);
		}

		if (isGameover)
		{
			FontAsset(U"TitleFontShadow")(U"GameOver").drawAt(Scene::Width() / 2, Scene::Height() / 2, ColorF(Palette::Black));
			FontAsset(U"TitleFont")(U"GameOver").drawAt(Scene::Width() / 2, Scene::Height() / 2, ColorF(Palette::Red));

			FontAsset(U"MenuFontShadow")(U"スペースキーでタイトルに戻る").drawAt(Scene::Width() / 2, Scene::Height() / 2 + 100, ColorF(Palette::Black));
			FontAsset(U"MenuFont")(U"スペースキーでタイトルに戻る").drawAt(Scene::Width() / 2, Scene::Height() / 2 + 100, ColorF(Palette::Red));
		}
	}

	std::pair<int, int> getEyePos(bool isRight, int gridInterval, int direction) const
	{
		std::pair<int, int> leftEye, rightEye;

		if (direction == 0)	// 正面
		{
			leftEye.first = gridInterval / 4.0;
			leftEye.second = gridInterval / 4.0;

			rightEye.first = gridInterval / 4.0 * 3.0;
			rightEye.second = gridInterval / 4.0;
		}
		if (direction == 1)	// 右
		{
			leftEye.first = gridInterval / 4.0 * 3.0;
			leftEye.second = gridInterval / 4.0;

			rightEye.first = gridInterval / 4.0 * 3.0;
			rightEye.second = gridInterval / 4.0 * 3.0;
		}
		if (direction == 2) // 下
		{
			leftEye.first = gridInterval / 4.0 * 3.0;
			leftEye.second = gridInterval / 4.0*3.0;

			rightEye.first = gridInterval / 4.0;
			rightEye.second = gridInterval / 4.0 * 3.0;
		}
		if (direction == 3) // 左
		{
			leftEye.first = gridInterval / 4.0;
			leftEye.second = gridInterval / 4.0*3.0;

			rightEye.first = gridInterval / 4.0;
			rightEye.second = gridInterval / 4.0;
		}

		if (isRight) return rightEye;
		else return leftEye;
	}
};

void Main()
{
	FontAsset::Register(U"TitleFont", 120, Typeface::Heavy);
	FontAsset::Register(U"TitleFontShadow", 120, Typeface::Black);
	FontAsset::Register(U"MenuFont", 80, Typeface::Bold);
	FontAsset::Register(U"MenuFontShadow", 80, Typeface::Black);
	FontAsset::Register(U"RankScoreFont", 80, Typeface::Heavy);

	constexpr Size windowSize = DisplayResolution::FHD_1920x1080;

	Scene::Resize(windowSize);
	(void)Window::SetFullscreen(true, unspecified, WindowResizeOption::KeepSceneSize);

	// シーンマネージャー
	App manager;

	manager.add<Title>(U"Title");
	manager.add<Game>(U"Game");
	manager.add<Help>(U"Help");
	manager.add<Ranking>(U"Ranking");

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}