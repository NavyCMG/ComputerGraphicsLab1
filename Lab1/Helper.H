#pragma once
#include <iostream>
#include <vector>
#include <string>

namespace Helper
{
    /* include memory leak detection in main
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetBreakAlloc(-1);
    _CrtDumpMemoryLeaks();*/

    int RNJesus(int rngMin, int rngMax)
    {
        //srand(time(NULL)); needed to seeed random in main
        return rand() % (rngMax + 1 - rngMin) + rngMin;
    }
    
    void ClearBuffer()
    {
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
    }

    int IntVal()
    {
        int val;
        std::cin >> val;

        while (std::cin.fail())
        {
            ClearBuffer();
            std::cout << "I'm sorry, that wasn't a valid input. Please input an integer." << std::endl;
            std::cin >> val;
        }
        ClearBuffer();
        return val;
    }
	int GetValidatedInt(const char* strMessage, int nMinimumRange = 0, int nMaximumRange = 0)
	{
		std::cout << strMessage;
        int input = IntVal();
        bool loop = true;
        while (loop = true)
        {
            if (nMinimumRange == 0 && nMaximumRange == 0)
            {
                loop = false;
                return input;
            }
            else if (input < nMinimumRange || input > nMaximumRange)
            {
                std::cout << "I'm sorry, that input is out of range, please try again." << std::endl;
                input = IntVal();
            }
            else
            {
                loop = false;
                return input;
            }
        }
	}
    int Menu(std::vector<std::string> options)
    {
        bool loop = true;  
        int input;
        for (int i = 0; i < options.size(); i++)
        {
           std::cout << options[i] << std::endl;
        }
        input = GetValidatedInt("Please select an option.", 1, options.size());
        system("cls");
        return input;
    }

    bool MultChoice(std::string question, std::vector<std::string> answers, int ans)
    {
        bool correct = false;
        std::cout << question << '\n';
        for (int i = 0; i < answers.size(); ++i)
        {
            std::cout << (i + 1) << ") " << answers[i] << '\n';
        }
        int resp = GetValidatedInt("", 1, answers.size());
        if (resp == ans)
        {
            correct = true;
            std::cout << "Correct\n";
        }
        else
        {
            std::cout << "Incorrect\n";
        }
        return correct;
    }
}