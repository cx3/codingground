// Example program

/*
Ostatnie zmiany

22-01-2016
*/


#ifndef INTEGER_HPP_INCLUDED
#define INTEGER_HPP_INCLUDED

#include <deque>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <algorithm>


class Integer
{
private:

    class IntegerException
    {
        public:
            IntegerException(){}
            IntegerException(std::string msg){ std::cout<<msg; }
    };
    /**---------------------------------------------------------*/


    std::deque<char*>&  fromString(std::string input)
    {
        std::deque<char*> *result = new std::deque<char*>{};
        size_t stop = input.size();
        for (size_t i=0;  i<stop;  ++i)
            if (input[i] < '0' || input[i] > '9')
                throw new IntegerException;
            else
                result->push_back(new char(input[i]));
        return *result;
    }


    inline std::string int2str(long long int input)
    {
        std::stringstream res{};
        res << input;
        return res.str();
    }


    inline std::string deq2str(std::deque<char*> &deq)
    {
        std::string result{""};
        size_t stop = deq.size();
        result.resize(stop);
        for (size_t i=0;  i<stop;  ++i)
            result[i] = * deq[i];
        return result;
    }


    inline void addZeros(std::deque<char*> &deq, size_t count = 1)
    {
        if (count==0) return;
        for (size_t s=0;  s<count;  ++s)
            deq.push_front(new char(48));
    }


    inline void clearDeq(std::deque<char*> &deq)
    {
        size_t s = deq.size();
        for (size_t i = 0;  i<s;  ++i)
        {
            char *c = deq[i];
            delete c;
        }
        deq.clear();
    }


    std::string trim(std::string input)
    {
        int pos = -1;
        for (size_t i=0;  i<input.size();  ++i)
            if (input[i]==48 || input[i]==0x0)
                pos = i;
            else
                break;
        if (pos != -1)
            return input.substr(pos+1, input.size());
        return input;
    }

    std::deque<char*> integer; /** number stored here **/

public:

    /** CONSTRUCTORS **/

    Integer()
    {
        integer.push_back(new char('0'));
    }


    Integer(const Integer &other)
    {
        const size_t s = other.integer.size();
        integer.resize(s);
        for (size_t i = 0;  i<s;  ++i)
            integer[i] = new char(* other.integer[i]);
    }

    ~Integer()
    {
        clearDeq(integer);
    }


    Integer(std::string input)
    {
        integer = fromString(trim(input));
    }


    Integer(long long int input)
    {
        integer = fromString(int2str(input));
    }


    /** OVERLOADED OPERATORS **/


    Integer&  operator+= (int adder)
    {
        std::string
            a = deq2str(integer),
            b = int2str(adder),
            c = "";
        int
            ax = a.size(),
            bx = b.size();
        if (ax > bx)
            for (int i = 0;  i<ax-bx;  ++i)
                b = "0" + b;
        if (bx > ax)
            for (int i = 0;  i<bx-ax;  ++i)
                a = "0" + a;
        ax = a.size();
        c.resize(ax+1);
        for (int i = 0;  i < ax+1;  ++i) c[i] = 0x0;
        for (int i = 0;  i < ax;  ++i) c[i+1] = a[i] + b[i] - 96;
        int cx = c.size();
        for (int tmp = 0;  tmp<cx;  ++tmp)
            for (int i = cx;  i != 0;  --i)
                if (c[i] > 9)
                {
                    c[i]  -= 10;
                    c[i-1]++;
                }
        for (int i = 0;  i<cx;  ++i)  c[i] += 48;
        clearDeq(integer);
        integer = fromString(c);
        return *this;
    }


    Integer&  operator+= (Integer &adder)
    {
        std::string
            a = deq2str(integer),
            b = deq2str(adder.integer),
            c = "";
        int
            ax = a.size(),
            bx = b.size();
        if (ax > bx)
            for (int i = 0;  i<ax-bx;  ++i)
                b = "0" + b;
        if (bx > ax)
            for (int i = 0;  i<bx-ax;  ++i)
                a = "0" + a;
        ax = a.size();
        c.resize(ax+1);
        for (int i = 0;  i < ax+1;  ++i) c[i] = 0x0;
        for (int i = 0;  i < ax;  ++i) c[i+1] = a[i] + b[i] - 96;
        int cx = c.size();
        for (int tmp = 0;  tmp<cx;  ++tmp)
            for (int i = cx;  i != 0;  --i)
                if (c[i] > 9)
                {
                    c[i] -= 10;
                    c[i-1]++;
                }
        for (int i = 0;  i<cx;  ++i)  c[i] += 48;
        clearDeq(integer);
        integer = fromString(c);
        return *this;
    }


    Integer&  operator*= (Integer &adder)
    {
        std::deque<char*>
            A = this->integer,
            B = adder.integer;
        size_t
            a = A.size()-1,
            b = B.size()-1;
        if (a>b)
        {
            std::swap(A,B);
            std::swap(a,b);
        }
        std::deque<std::deque<int>> lines{};
        for (int x = a;  x != -1;  --x)
        {
            std::deque<int> line{};
            for (int y = b;   y!=-1;  --y)
                line.push_front(((*A[x])*(*B[y]) - 48*48)%48);
            lines.push_back(line);
        }
        for (int x = lines.size()-1;  x != -1;  --x)
        {
            std::deque<int> line{lines[x]};
            for (int y = line.size()-1;  y != -1;  --y)
                if (line[y]>=10)
                {
                     int tens  = line[y]/10;
                     line[y]   -= tens*10;
                     line[y-1] += tens;
                }
            if (line[0]>=10)
            {
                 int tens  = line[0]/10;
                 line[0]   -= tens*10;
                 line.push_front(tens);
            }
            lines[x].clear();
            lines[x] = line;
        }
        size_t s = lines.size();
        for (size_t x = 0;  x < lines.size();  ++x)
        {
            for (size_t y = 0;  y < x;  ++y)
                lines[x].push_back(0);
            for (size_t y = s-1;  y != x;  --y)
                lines[x].push_front(0);
        }

        /** wyswietlenie **/
        for (size_t x = 0;  x < lines.size();  ++x)
        {
            std::deque<int> line = lines[x];
            for (size_t y = 0;  y < line.size();  ++y)
                std::cout<<line[y]<<" ";
            std::cout<<"\n";
        }
        std::cout<<"\n-----------------------------\n";

        std::deque<int> result{};
        result.resize(lines[0].size()+1,0);
        
        for (size_t x = 0;  x < lines.size();  ++x)
        {
            auto line = lines[x];
            
            for (size_t y = 0;  y < line.size();  ++y)
                result[y] += line[y];
        }
        
        for (size_t x = result.size()-1;  x>0;  --x)         
            if (result[x]>=10)
            {
                int tens   = result[x]/10;
                result[x] -= tens*10;
                result[x-1] += tens;
            }
        if (result[0]>=10)
        {
            int tens = result[0]/10;
            result[0] = tens*10;
            result.push_front(tens);
        }
        
        std::cout<<"\nRESULT:\n";
        for (size_t x = 0;  x < result.size();  ++x)
            std::cout << result[x] << " ";
        std::cout<<"\n\n";
        
        std::string result_str{""};
        result_str.resize(result.size());
        
        for (size_t x = 0;  x < result.size();  ++x)
            result_str[x] = result[x];
        integer = Integer(result_str).integer;
        return *this;
    }


    Integer&  operator++()
    {
        (*this)+=1;
        return (*this);
    }


    /** LESS/MORE/EQUAL OPERATORS **/

    bool operator< (Integer other) const
    {
        int
            a = this->integer.size(),
            b = other.integer.size();
        if (a<b) return true;
        if (a==b)
            if (* this->integer[0] < * other.integer[0])
                return true;
        return false;
    }


    bool operator> (Integer other) const
    {
        return (*this) < other;
    }


    bool operator==(Integer other) const
    {
        int
            a = this->integer.size(),
            b = other.integer.size();
        if (a==b)
            if (* this->integer[0] == * other.integer[0])
                return true;
        return false;
    }


    bool operator!=(Integer other) const
    {
        if (*this == other) return true;
        return false;
    }


    bool operator<=(Integer other) const
    {
        if (*this < other || *this == other) return true;
        return false;
    }


    bool operator>=(Integer other) const
    {
        if (*this > other || *this == other) return true;
        return false;
    }


    operator std::string()
    {
        std::string result{""};
        size_t stop = integer.size();
        result.resize(stop+1);
        for (size_t i=0;  i<stop;  ++i)
            result[i] = * integer[i];
        return trim(result);
    }


    std::string toString()
    {
        //return (std::string) (*this);
        std::string result{""};
        size_t stop = integer.size();
        result.resize(stop+1);
        for (size_t i=0;  i<stop;  ++i)
            result[i] = * integer[i];
        return trim(result);
    }
};

#endif // INTEGER_HPP_INCLUDED