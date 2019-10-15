/***********************************************************
CSCI 480 - Assignment 4 - Fall 2019

Progammer: Sam Piecz
Z-ID: Z1732715
Section: 2 
TA: Jingwan Li  
Date Due: Oct 11, 2019 Purpose: Priority Scheduling Simulation. 
************************************************************/
#include "Process.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <queue>

using std::string;
using std::vector;
using std::endl;
using std::cout;
using std::cerr;
using std::getline;
using std::istringstream;
using std::ifstream;
using std::stringstream;
using std::stoi;
using std::queue;
using std::priority_queue;

vector<string> stringToVector(const string& str)
{
  string buf;                 // Have a buffer string
  stringstream ss(str);       // Insert the string into a stream
  vector<string> tokens; // Create vector to hold our words
  while (ss >> buf)
  {
    tokens.push_back(buf);
  }

  return tokens;
}

vector<Process*> processFile(const string& filename)
{
  // Instantiate infile and line parser.
  vector<Process*> processes;
  ifstream infile(filename);
  int processCount = -1;
  bool stopHere = false;
  string line;

  // While we have a line in the file iterate. 
  while (getline(infile, line) && !stopHere)
  {

    // Stop when loudly asked to stop.
    if (line.find("STOPHERE") != string::npos)
    {
      stopHere = true;
    }

    // Variables to help process loop
    bool cpuBurstData;

    // Get the line, that pesky bugger.
    istringstream iss(line);

    // Process name or cpu burst data? 
    if (line.length() >= 20)
    {
      cpuBurstData = true;
    }
    else
    {
      cpuBurstData = false;
    }


    // Loop line by line.
    if (!stopHere)
    {

      string theName, thePriority, theArrivalTime;
      // if not burst then new process create it
      if (!cpuBurstData)
      {

        cout << "not burst data" << endl;
        int count = 0;
        for (auto x : stringToVector(line)) 
        {
          if (count == 0)
          {
            theName = x;
          }
          if (count == 1)
          {
            thePriority = x;
          }
          if (count == 2)
          {
            theArrivalTime = x;
          }
          count++;
        }

				// Create new process and set vars
        Process* theProcess = new Process(); 
				theProcess->setName(theName);
				theProcess->setPriority(stoi(thePriority));
				theProcess->setArrivalTime(stoi(theArrivalTime));
        theProcess->setPID(processCount);
        processes.push_back(theProcess);
        processCount++;
        cout << "Process print: " << endl; 
        theProcess->print();
        cout << "Processes print: " << endl; 
        processes[processCount]->print();

      }
      // if burst data assign data to most recent process
      else 
      {
        cout << "cpu burst data" << endl;
        string letter;
        int value;
        int count1 = 0;
        int trueCount = 0;

        for (string x : stringToVector(line)) 
        {
          if (count1 == 0)
          {
            letter = x;
            cout << "Letter: " << letter << endl;
          }
          else if (count1 == 1)
          {
            value = stoi(x);
            cout << "Value: " << value << endl;
            count1 = -1;
            processes[processCount]->setHistory(letter, value, trueCount);
            trueCount++;
          }
          count1++;
        }

      }
    }

  }

  return processes;

}

queue<Process*> loadQueue(vector<Process*> _processes)
{

  queue<Process*> _queue;
  for (auto x : _processes)
    _queue.push(x);

  return _queue;

}

int main()
{
  // Constants
  int MAX_TIME = 500;
  int IN_PLAY = 6;
  int QUEUE_SIZE = 20;
  int ARRAY_SIZE = 10;
  int HOW_OFTEN = 25;

  // Instantiate vars.
  queue<Process*> entryQueue;
  priority_queue<Process*> Ready;
  priority_queue<Process*> Input;
  priority_queue<Process*> Output;
  Process* Active;
  Process* IActive;
  Process* OActive;
  int timer = 0;
  int processesInUse = 0;


  // Process file, generate processes, and pack into queue.
  entryQueue = loadQueue(processFile("data4.txt"));
		
  // 3. 
  while(timer < MAX_TIME)
  {
		
		// a.
    if (entryQueue.size() < IN_PLAY)
    {
      if (entryQueue.front()->arrivalTime < timer)
      {
        Ready.push(entryQueue.front());
      }
    }

		// b.
    if (Active == NULL)
    {
      if (Ready.empty())
      {
				timer++;
			  continue;	
      }
			else
			{
				Active = Ready.top();
				Ready.pop();
			}
    }
		// c.
		else
		{
			if (Active->history[Active->sub].letter == "I")
			{
				Input.push(Active);	
			}
			else if (Active->history[Active->sub].letter == "O")
			{
				Output.push(Active);	
			}
			else if (Active->history[Active->sub].letter == "N")
			{
				delete Active;
				continue;
			}
			Active->incrementSub();
		}

		// d.
		if (IActive == NULL)
		{
			if (!Input.empty())
			{
				IActive = Input.top();	
			}
		}
		// e.
		else	
		{
			if (IActive->sub == 9)
			{
				Ready.push(IActive);
			}
		}

		// f.
		if (OActive == NULL)
		{
			if (!Output.empty())
			{
				OActive = Output.top();
			}
		}
		// g.
		else 
		{
			if (OActive->sub == 9)
			{
				Ready.push(OActive);
			}
		}

    timer++;

		// h.
		if ( timer % HOW_OFTEN)
		{
			// call prints
			if (Active != NULL)
			{
				Active->print();
			}
			if (IActive != NULL)
			{
				IActive->print();
			}
			if (OActive != NULL)
			{
				OActive->print();
			}

			while (!entryQueue.empty())
			{
				entryQueue.front()->print();
				entryQueue.pop();
			}

			while (!Ready.empty())
			{
				Ready.top()->print();
				Ready.pop();
			}

			while (!Input.empty())
			{
				Input.top()->print();
				Input.pop();
			}

			while (!Output.empty())
			{
				Output.top()->print();
				Output.pop();
			}

		}

  }

  return 0;

}
