#include "Queue.h"
#include "Frame.h"
#include <vector>

/*
In the simplest case, we associate with each Frame-table entry a
time-of-use field and add to the CPU a logical clock or counter. The clock is
incremented for every memory reference. Whenever a reference to a Frame
is made, the contents of the clock register are copied to the time-of-use
field in the Frame-table entry for that Frame.
*/

class FrameTable
{
private:
    ArrayQueue<Frame> q;
    int fault_count;
    // Check if the frame is valid
    bool IsValid(Frame f, int type);
    // Decide how many frames to give to a process
    int FrameAllocation(int type, std::vector<int> f_ref);
    // Decide when a Page Fault Occurs
    int PageReplacement(int type, std::vector<int> f_ref);
    int PageSelect();
    // Index of RAM
    int p_mem_index;
public:
    FrameTable(int p_num);
    ~FrameTable();
    int GetTotalFaults() { return fault_count; }
    void FrameRead(std::vector<int> f_ref);
    void FrameWrite(std::vector<int> f_ref);
    void FrameRW(std::vector<int> f_ref);
    void PrintTable();
};

int FrameTable::FrameAllocation(int type, std::vector<int> f_ref)
{
    // In the example, each process is given one frame
    // but multiple frames could be allocated
    return PageReplacement(type, f_ref);
}
int FrameTable::PageReplacement(int type, std::vector<int> f_ref)
{
    int f_num = f_ref.at(0);
    Frame* f = q.at(f_num);
    // Decide when page replacement is required
    if(f->GetV() == INVALID)
    {
        // Select the frames that are to be replaced; Page Fault
        fault_count++;
        cout << "Fault: " << f_num << " page evicted." << endl;
        int i = PageSelect();
        return (i == -1) ? (f_num) : (i);
    }
    else
    {
        cout << "Hit: page exists in memory." << endl;
    }
    return f_num;
}

int FrameTable::PageSelect()
{
    /*
    LRU chooses the page that has not been used for the longest period of time.
    Whenever a page is referenced, it is removed from the back and put on the front.
    */
   if(q.peekBack().GetIndex() != -1)
   {
       Frame f = q.dequeue();
       int i = q.enqueue(f);
       return i;
   }
   return -1;
}

FrameTable::FrameTable(int p_num) : fault_count(0), p_mem_index(0)
{
    for(int i = 0; i < p_num; ++i)
    {
        Frame f;
        f.SetT();
        q.enqueue(f);
    }
}

FrameTable::~FrameTable()
{
    fault_count = 0;
}



/*
Write contents of frame to memory
*/
void FrameTable::FrameRead(std::vector<int> f_ref)
{
    Frame* f = q.at(FrameAllocation(R, f_ref));
    // Frame is being modified
    f->SetV(VALID);
    f->SetM(0);
    f->SetKey(f_ref[0]);
    f->SetR(1);
    f->SetDur(f_ref[2]);
    f->SetIndex(p_mem_index++);
}
/*
Read contents of memory into Frame; Eviction
*/
void FrameTable::FrameWrite(std::vector<int> f_ref)
{
    Frame* f = q.at(FrameAllocation(W, f_ref));
    if(f == NULL)
        cout << "frame is empty!\n";
    // Frame is being referenced
    f->SetV(VALID);
    f->SetKey(f_ref[0]);
    f->SetR(1);
    f->SetDur(f_ref[2]);
    f->SetM(1);
    f->SetIndex(p_mem_index++);
}
/*
Read contents of memory into Frame and write changes into memory
*/
void FrameTable::FrameRW(std::vector<int> f_ref)
{
    Frame* f = q.at(FrameAllocation(RW, f_ref));
    // Frame is being referenced
    f->SetR(1);
    // Frame is being modified
    f->SetM(1);
    f->SetV(VALID);
    f->SetIndex(p_mem_index++);
}

void FrameTable::PrintTable()
{
    q.PrintQueue();
}