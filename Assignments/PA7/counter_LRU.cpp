

#include "FrameTable.h"

/*
Number of Virtual Pages
Number of Page Frames Available (not used by page table)
Number of References
*/
int GetInput(char* msg)
{
    int val;
    cout << msg << endl;
    cin >> val;
    return val;
}
std::vector<std::vector<int>> GetRef(int n)
{
    std::vector<std::vector<int>> ref;
    for(int i = 0; i < n; ++i)
    {
        std::vector<int> ref_n;
        cout << "Enter Reference Frame " << i + 1 << endl;
        for(int j = 0; j < 3; ++j)
        {
            int a;
            cin >> a;
            ref_n.push_back(a);
        }
        ref.push_back(ref_n);
    }
    return ref;
}

int main()
{
    // Example data
    int v_pages = GetInput("Enter Number of Virtual Pages");
    int p_frames = GetInput("Enter Number of Page Frames Available");
    int p_free = GetInput("Enter Number of References");
    std::vector<std::vector<int>> ref_frames = GetRef(p_frames);
    // Print the input
    cout << "\nInput Provided:\n";
    cout << v_pages << "\n" << p_frames<< "\n" << p_free << endl;
    for(int i = 0; i < p_frames; ++i)
    {
        cout << ref_frames[i][0] << ", " << ref_frames[i][1] << ", " << ref_frames[i][2] << endl;
    }
    // Process Input
    if(p_frames + p_free == v_pages)
    {
        FrameTable ft(v_pages);
        for(int i = 0; i < p_frames; ++i)
        {
            if(ref_frames.at(i).at(1) == 0)
            {
                ft.FrameRead(ref_frames.at(i));
            }
            else
            {
                ft.FrameWrite(ref_frames.at(i));
            }
        }
        cout << "Total number of faults is " << ft.GetTotalFaults() << endl;
    }
    return 0;
}