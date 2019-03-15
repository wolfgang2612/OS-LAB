#include <bits/stdc++.h>

using namespace std;

int main(int argc, char const *argv[])
{
	ifstream infile("page.txt");

	int n, cur, faults, skip;

	while(infile >> n)
	{
		vector<int> vals(100, INT_MAX), page(100, -1), v(100);
		int i = 0, cur = 0, faults = 0;
		while(1)
		{
			infile >> v[i];
			if(v[i] == -1)
				break;
			i++;
		}
		
		int k = 0;

		//LRU
		while(v[k] != -1)
		{
			skip = 0;
			for(int i = 0;i < n;i++)
			{
				if(page[i] == v[k])
				{
					skip = 1;
					vals[i] = cur;
					cur++;
				}
			}

			if(!skip)
			{
				faults++;
				int min = INT_MAX, mini = INT_MAX;
				for(int i = 0;i < n; i++)
				{
					if(vals[i] == INT_MAX)
					{
						mini = i;
						break;
					}
					if(vals[i] < min)
					{
						mini = i;
						min = vals[i];
					}
				}
				page[mini] = v[k];
				vals[mini] = cur;
				cur++;
			}
			cout << "Queue after requesting " << v[k] << ": ";
				for(int i = 0;i < n; i++)
					cout << page[i] << " ";
				cout << "\n";
			k++;
		}
		cout << "Scheme\t\tFaults\nLRU\t\t" << faults << "\n";
		cout << "----------------\n";

		k = 0;
		//LFU
		map<int, int> m;
		faults = 0;
		vector<int> page2(100, -1);
		while(v[k] != -1)
		{
			m[v[k]]++;
			skip = 0;
			for(int i = 0;i < n;i++)
			{
				if(page2[i] == v[k])
				{
					skip = 1;
					break;
				}
			}

			if(!skip)
			{
				faults++;
				int min = INT_MAX, mini = INT_MAX;
				for(int i = 0;i < n; i++)
				{
					if(page2[i] == -1)
					{
						mini = i;
						break;
					}
					if(m[page2[i]] < min)
					{
						mini = i;
						min = m[page2[i]];
					}
				}
				page2[mini] = v[k];
			}

			cout << "Queue after requesting " << v[k] << ": ";
				for(int i = 0;i < n; i++)
					cout << page2[i] << " ";
				cout << "\n";
			k++;
		}
		cout << "Scheme\t\tFaults\nLFU\t\t" << faults << "\n";
		cout << "----------------\n";

		k = 0;
		//Optimal
		faults = 0;
		vector<int> page3(100, -1);
		while(v[k] != -1)
		{
			skip = 0;
			for(int i = 0;i < n;i++)
			{
				if(page3[i] == v[k])
				{
					skip = 1;
					break;
				}
			}

			if(!skip)
			{
				faults++;
				int maxi, maxd = 0;
				for(int i = 0;i < n;i++)
				{
					int j = k;

					if(page3[i] == -1)
					{
						maxi = i;
						break;
					}

					while(v[j] != -1)
					{
						if(page3[i] == v[j])
						{
							if(maxd < j - k)
							{
								maxi = i;
								maxd = j - k;
							}
							break;
						}
						j++;
					}

					if(v[j] == -1)
					{
						maxi = i;
						break;
					}
				}
				page3[maxi] = v[k];
			}
			cout << "Queue after requesting " << v[k] << ": ";
				for(int i = 0;i < n; i++)
					cout << page3[i] << " ";
				cout << "\n";
			k++;
		}
		cout << "Scheme\t\tFaults\nOptimal\t\t" << faults << "\n";
		cout << "----------------\n";
	}
	return 0;
}