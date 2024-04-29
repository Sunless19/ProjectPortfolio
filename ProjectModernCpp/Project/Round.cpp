module round;

using gartic::Round;

Round::Round()
{

}


Round::Round(int noRounds)
    :m_noRounds(noRounds)
{
}

Round::Round(const Round& other)
    : m_noRounds(other.m_noRounds)
{
}

int Round::GetNoRounds() const
{
    return m_noRounds;

}

void Round::SetNoRounds(int noRounds)
{
    m_noRounds = noRounds;
}

Round::Round(Round&& other) noexcept
    : m_noRounds(other.m_noRounds)
{
    other.m_noRounds = 0;
}

Round& Round::operator=(Round&& other) noexcept
{
    if (this != &other) {
        m_noRounds = other.m_noRounds;
        other.m_noRounds = 0;
    }
    return *this;
}

Round& Round::operator=(const Round& other)
{
    if (this != &other) {
        m_noRounds = other.m_noRounds;
    }
    return *this;
}





