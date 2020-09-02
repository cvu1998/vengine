#include "Tic-Tac-Toe.h"

TTTGrid::TTTGrid()
{
}

TTTGrid::~TTTGrid()
{
}

bool TTTGrid::isWinningMove(size_t index, uint32_t value) const
{
    return isWinningRow(index, value) || isWinningColumn(index, value) ||
        isWinningRightDiagonal(index, value) || isWinningLeftDiagonal(index, value);
}

void TTTGrid::getCurrentStateCode(std::string& string)
{
    std::stringstream ss;
    for (int i = 0; i < 3; i++)
        ss << getValue(i, 0) << getValue(i, 1) << getValue(i, 2);
    string = ss.str();
}

void TTTGrid::printGrid()
{
    ELY_INFO("----------");
    for (int i = 2; i >= 0; i--)
        ELY_INFO("{0} {1} {2}", getValue(i, 0), getValue(i, 1), getValue(i, 2));
    ELY_INFO("----------");
}

bool TTTGrid::isWinningRow(size_t index, uint32_t value) const
{
    uint16_t counter = 1;
    int32_t row = (int32_t)index / 3;
    int32_t column = (int32_t)index % 3;
    if (getValue(row, column + 1) == value)
    {
        counter++;
        if (getValue(row, column + 2) == value)
            counter++;
    }
    if (getValue(row, column - 1) == value)
    {
        counter++;
        if (getValue(row, column - 2) == value)
            counter++;
    }
    return counter >= 3;
}

bool TTTGrid::isWinningColumn(size_t index, uint32_t value) const
{
    uint16_t counter = 1;
    int32_t row = (int32_t)index / 3;
    int32_t column = (int32_t)index % 3;
    if (getValue(row + 1, column) == value)
    {
        counter++;
        if (getValue(row + 2, column) == value)
            counter++;
    }
    if (getValue(row - 1, column) == value)
    {
        counter++;
        if (getValue(row - 2, column) == value)
            counter++;
    }
    return counter >= 3;
}

bool TTTGrid::isWinningRightDiagonal(size_t index, uint32_t value) const
{
    uint16_t counter = 1;
    int32_t row = (int32_t)index / 3;
    int32_t column = (int32_t)index % 3;
    if (getValue(row + 1, column + 1) == value)
    {
        counter++;
        if (getValue(row + 2, column + 2) == value)
            counter++;
    }
    if (getValue(row - 1, column - 1) == value)
    {
        counter++;
        if (getValue(row - 2, column - 2) == value)
            counter++;
    }
    return counter >= 3;
}

bool TTTGrid::isWinningLeftDiagonal(size_t index, uint32_t value) const
{
    uint16_t counter = 1;
    int32_t row = (int32_t)index / 3;
    int32_t column = (int32_t)index % 3;
    if (getValue(row + 1, column - 1) == value)
    {
        counter++;
        if (getValue(row + 2, column - 2) == value)
            counter++;
    }
    if (getValue(row - 1, column + 1) == value)
    {
        counter++;
        if (getValue(row - 2, column + 2) == value)
            counter++;
    }
    return counter >= 3;
}

MinimaxPlayer::MinimaxPlayer(TTTGrid* grid) : m_Grid(grid)
{
}

size_t MinimaxPlayer::playAction()
{
    State origin = {0, *m_Grid };
    generateChildren(origin.Children, origin, Minimax, false);

    for (State& s : origin.Children)
    {
        generateChildren(s.Children, s, Opponent, true);
    }

    for (State& s : origin.Children)
    {
        if (!s.Children.empty())
        {
            State minState = *std::min_element(s.Children.begin(), s.Children.end());
            s.Value = minState.Value;
        }
    }

    size_t actionIndex = 0;
    size_t index = 0;
    origin.Value = origin.Children[0].Value;
    for (size_t i = 1; i < origin.Children.size(); i++)
    {
        if (origin.Children[i].Value > origin.Value)
        {
            origin.Value = origin.Children[i].Value;
            index = i;
        }
    }

    for (size_t i = 0; i < 9; i++)
    {
        if (origin.StateRep.Grid[i] != origin.Children[index].StateRep.Grid[i])
        {
            actionIndex = i;
            break;
        }
    }

    return actionIndex;
}

int MinimaxPlayer::evaluateState(const TTTGrid& grid)
{
    int value = 0;
    for (int32_t i = 0; i < 3; i++)
    {
        int exponent = 0;
        for (int32_t j = 0; j < 3; j++)
        {
            if (grid.getValue(i, j) == Minimax)
                exponent++;
            else if (grid.getValue(i, j) == Opponent)
                exponent--;
        }
        if (exponent > 0)
        {
            int gain = 1;
            for (int z = 1; z < exponent; z++)
                gain += 10 * z;

            value += gain;
        }
        else if (exponent < 0)
        {
            int loss = -1;
            for (int z = -1; z > exponent; z--)
                loss += 10 * z;

            value += loss;
        }
    }

    for (int32_t i = 0; i < 3; i++)
    {
        int exponent = 0;
        for (int32_t j = 0; j < 3; j++)
        {
            if (grid.getValue(j, i) == Minimax)
                exponent++;
            else if (grid.getValue(j, i) == Opponent)
                exponent--;
        }
        if (exponent > 0)
        {
            int gain = 1;
            for (int z = 1; z < exponent; z++)
                gain *= 10;

            value += gain;
        }
        else if (exponent < 0)
        {
            int loss = -1;
            for (int z = -1; z > exponent; z--)
                loss *= 10;

            value += loss;
        }
    }
    value += evaluateDiagonals(grid);

    return value;
}

int MinimaxPlayer::evaluateDiagonals(const TTTGrid& grid)
{
    int value = 0;
    int exponent = 0;
    if (grid.getValue(0, 0) == Minimax)
        exponent++;
    else if (grid.getValue(0, 0) == Opponent)
        exponent--;
    if (grid.getValue(1, 1) == Minimax)
        exponent++;
    else if (grid.getValue(1, 1) == Opponent)
        exponent--;
    if (grid.getValue(2, 2) == Minimax)
        exponent++;
    else if (grid.getValue(2, 2) == Opponent)
        exponent--;
    if (exponent > 0)
    {
        int gain = 1;
        for (int z = 1; z < exponent; z++)
            gain *= 10;

        value += gain;
    }
    else if (exponent < 0)
    {
        int loss = -1;
        for (int z = -1; z > exponent; z--)
            loss *= 10;

        value += loss;
    }

    exponent = 0;
    if (grid.getValue(2, 0) == Minimax)
        exponent++;
    else if (grid.getValue(2, 0) == Opponent)
        exponent--;
    if (grid.getValue(1, 1) == Minimax)
        exponent++;
    else if (grid.getValue(1, 1) == Opponent)
        exponent--;
    if (grid.getValue(0, 2) == Minimax)
        exponent++;
    else if (grid.getValue(0, 2) == Opponent)
        exponent--;
    if (exponent > 0)
    {
        int gain = 1;
        for (int z = 1; z < exponent; z++)
            gain *= 10;

        value += gain;
    }
    else if (exponent < 0)
    {
        int loss = -1;
        for (int z = -1; z > exponent; z--)
            loss *= 10;

        value += loss;
    }
    return value;
}

void MinimaxPlayer::generateChildren(std::vector<State>& states, const State& state, uint32_t turn, bool lastLayer)
{
    for (size_t i = 0; i < 9; i++)
    {
        TTTGrid newGrid = state.StateRep;
        if (newGrid.isValid(i))
        {
            newGrid.Grid[i] = turn;
            switch (lastLayer)
            {
            case true:
                states.emplace_back(evaluateState(newGrid), newGrid);
                break;
            case false:
                states.emplace_back(0, newGrid);
                break;
            }
        }
    }
}