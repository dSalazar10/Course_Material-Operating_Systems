#include <iostream>
#include <ctime>

#ifndef Frame_H
#define Frame_H

#define VALID   9
#define INVALID 13
#define R  0
#define W  1
#define RW 2

class Frame
{
private:
    /*
    Index to FrameTable
    */
    int key;
    /*
    1 if page has been referenced recently (read/write)
    reset r_bit to 0 after a set of 6 pages have been referenced
    */ 
    int r_bit;
    /*
    1 if page has been modified (needs to be written to disk)
    reset m_bit to 0 after a set of 6 pages have been referenced
    */
    int m_bit;
    /*
    If this bit is set to “VALID,” the associated page is both legal and in memory
    If this bit is set to “INVALID,” the page either is not valid or is valid but is currently on the disk.
    */
    int v_bit;
    /*
    Index to the block of memory in RAM
    */
    int index;
    /*
    Duration or number of clock ticks needed by a process
    */
    int dur;
    /*
    LRU replacement associates with each page the time of that page’s last use.
    */
    time_t time;
    
public:
    Frame();
    ~Frame();
    // Mutators
    void SetKey(int k)  { key = k; }
    void SetR(int r)    { r_bit = r; }
    void SetM(int m)    { m_bit = m; }
    void SetV(int v)    { v_bit = v; }
    void SetIndex(int f){ index = f; }
    void SetDur(int d)  { dur = d; SetT(); }
    void SetT()         { time = clock(); }
    int GetKey()        { return key; }
    int GetR()          { return r_bit; }
    int GetM()          { return m_bit; }
    int GetV()          { return v_bit; }
    int GetIndex()      { return index; }
    int GetDur()        { return dur; }
    time_t GetT()       { return time; }
    void Print();
    
};

#endif /* Frame_H */

Frame::Frame() : r_bit(0), m_bit(0), v_bit(INVALID), index(-1), time(std::time(nullptr))
{
}

Frame::~Frame()
{
    r_bit = 0;
    m_bit = 0;
    v_bit = INVALID;
    index = 0;
    time = 0;
}

void Frame::Print()
{
    int v = (GetV() == VALID) ? (1) : (0), i = GetIndex();
    i = (i == -1) ? (0) : (i);
    cout << GetR() << ", " << GetM() << ", " << v << ", " << i << endl;
}

