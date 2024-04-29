export module round;
import game;

namespace gartic
{
	export class Round :public Game
	{
	private:
		int m_noRounds;
		//static const int m_noSubrounds = Players.size();

	public:

		Round();
		Round(int m_noRounds);
		Round(const Round& other);

		int GetNoRounds() const;
		void SetNoRounds(int noRounds);

		Round(Round&& other) noexcept;
		Round& operator=(Round&& other) noexcept;
		Round& operator=(const Round& other);
	};
}