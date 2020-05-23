#include <string>

#include "format.h"

using std::string;
using std::to_string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {  
    if(seconds < 10 )
        return "00:00:0"+to_string(seconds);
    else if(seconds >= 10 && seconds < 60 )
        return "00::00::" + seconds;
    else if(seconds>=60 && seconds <3600) {
        int min = seconds/60;
        seconds = seconds % 60;
        string secString, minString;
        if(seconds/10 == 0){
            secString = "0"+to_string(seconds);
        }
        else{
            secString = to_string(seconds);
        }
        if(min/10 == 0) {
            minString = "0"+to_string(min);
        }
        else {
            minString = to_string(min);
        }

        return "00:"+minString+":"+secString;
    }
    else
    {
        long hours = seconds/3600;
        int min = (seconds%3600)/60;
        seconds = (seconds%3600)%60;
        string secString, minString, hourString;
        if(seconds/10 == 0){
            secString = "0"+to_string(seconds);
        }
        else{
            secString = to_string(seconds);
        }
        if(min/10 == 0) {
            minString = "0"+to_string(min);
        }
        else {
            minString = to_string(min);
        }
        if(hours/10 == 0){
            hourString ="0"+to_string(hours);
        }
        else{
            hourString = to_string(hours);
        }

        return hourString+":"+minString+":"+secString;
    }   
}