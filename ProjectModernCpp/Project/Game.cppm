export module game;
import user;
import <vector>;
import <string>;
import <algorithm>;
import <iostream>;
import <fstream>;
import <random>;
import <unordered_set>;
import <map>;

namespace gartic
{
	export class Game 
	{
	public:
		Game();
		~Game();

		const std::vector<User>& GetPlayers() const;
		const std::unordered_set<std::string>& GetUsedWords() const;
		std::vector<std::string>& GetWords();
		const std::vector<std::string>& GetTips() const;
		 std::map<std::string, std::vector<float>> GetScores() ;
		const User& GetDrawer() const;
		const int& GetCurrentRound(int);

		void SetPlayers(const std::vector<User> asd);
		//void SetPlayerScoreDefault(float, User);
		void SetScores(const std::map<std::string, std::vector<float>>& scor);
		void addScore(std::string, float);
		void SetDrawer(const User& user);

		std::string selectRandomWord(std::vector<std::string>& m_Words);
		void verifyGuessed();
		void AddUsedWord(const std::string&);
		void FileRead();
		void GameOver(int);
		double CalculateScoreGuesser(double);
		double CalculateScorePainter(double);

		//std::string GetGuessedWord() const;

		Game& operator=(const Game& other);//copy assigment
		Game(Game&& other) noexcept;//move constructor
		Game& operator=(Game&& other) noexcept;//move assigment
	private:
		static const int noRounds = 4;
		std::vector<User> m_Players;
		std::unordered_set<std::string> m_usedWords;
		std::vector<std::string> m_Words;
		std::map<std::string, std::vector<float>> m_Scores;// scor , playerId
		std::vector<std::string> m_Tips;
		User m_drawingPlayer;
		//std::string m_guessedWord;
	};
}