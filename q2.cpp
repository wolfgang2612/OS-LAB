#include <bits/stdc++.h>

#define m 4
#define n 5

using namespace std;

int main(int argc, char const *argv[])
{
	int avail[m] = {0};
	int max[n][m], alloc[n][m], need[n][m];

	for(int i = 0;i < n;i++)
		for(int j = 0;j < m;j++)
			max[i][j] = alloc[i][j] = need[i][j];

	cout << "Enter the allocation matrix:\n";
	for(int i = 0;i < n;i++)
		for(int j = 0;j < m;j++)
			cin >> alloc[i][j];

	cout << "Enter the max resource matrix:\n";
	for(int i = 0;i < n;i++)
		for(int j = 0;j < m;j++)
			cin >> max[i][j];

	cout << "Enter the availability matrix:\n";
	for(int i = 0;i < m;i++)
		cin >> avail[i];

	for(int i = 0;i < n;i++)
		for(int j = 0;j < m;j++)
			need[i][j] = max[i][j] - alloc[i][j];

	cout << "Part A: Need Matrix\n";

	for(int i = 0;i < n;i++)
	{
		for(int j = 0;j < m;j++)
			cout << need[i][j] << " ";
		cout << "\n";
	}	

	cout << "---------------------------\n";

	cout << "Part B: System safe state check\n";

	int work[m];
	int finish[n] = {0};

	copy(begin(avail), end(avail), begin(work));

	while(1)
	{
		int tobreak = 0;
		for(int i = 0;i < n;i++)
		{
			if(finish[i] == 0)
			{
				int counter = 0;
				for(int j = 0;j < m;j++)
				{
					if(need[i][j] <= work[j])
						counter++;
				}
				if(counter == m)
				{
					tobreak++;
					for(int j = 0;j < m;j++)
						work[j] += alloc[i][j];
					finish[i] = 1;
				}
			}
		}
		if(tobreak == 0)
			break;
	}

	int unsafe = 0;

	for(int i = 0;i < n; i++)
	{
		if(!finish[i])
		{
			cout << "System is unsafe.\n";
			unsafe = 1;
			break;
		}
	}

	if(!unsafe)
		cout << "System  is safe.\n";

	cout << "---------------------------\n";

	int request[] = {0, 4 ,2, 0};
	cout << "Part C: P1 asking for resources ";
	for(int i = 0;i < m;i++)
		cout << request[i] << " ";
	cout << "\n";

	int tobreak = 0;
	for(int i = 0;i < m;i++)
		if(request[i] > need[1][i])
			tobreak = 1;

	if(tobreak)cout << "Denied. Process request exceeds maximum claim.\n";

	else
	{
		tobreak = 0;
		for(int i = 0;i < m;i++)
			if(request[i] > avail[i])
				tobreak = 1;
		if(tobreak)cout << "Denied. Process must wait as resources are scarce.\n";

		else
		{
			cout << "Granted.\n";
			for(int i = 0;i < m;i++)
			{
				avail[i] -= request[i];
				alloc[1][i] += request[i];
				need[1][i] -= request[i];
			}
		}
	}

	cout << "---------------------------\n";
	return 0;
}