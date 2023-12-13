// 10920144
#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<fstream>
#include <sstream>
#include <algorithm>
#include<regex>
#include<stack>
#include<iterator>
using namespace std ;
struct Job {
    int pid ;
    int arrival ;
    int cpuburst ;
    int priority ;
} ;// struct 

struct Finishjob {
    int pid ;
    int arrival ;
    int turnaroundtime ;
    int finishtime ;
    int waittime ;
} ; // struct

class Ganttpidtranfer {
    public :
    string pidtransfer( int pid ) {
        if (  0 <= pid && pid <= 9 ) {
            stringstream ss ;
            ss << pid ;
            return ss.str() ;
        } // if
        else {
            char ch = 65 + ( pid - 10 ) ;
            string temp = "" ;
            temp += ch ; 
            return temp ;
        } // else 
    } // pidtransfer



}; // Ganttpidtranfer
class FCFS {
    public :
        vector < Job > list ;
        vector < Job > readyqueue ;
        vector < Finishjob > finishlist ;
        string ganttstr ;
        Ganttpidtranfer gtf ;
        void setJoblist( vector <Job> joblist ) { 
            list = joblist ;
            ganttstr = "" ;
        } // setJoblist
        FCFS() {}
        ~FCFS() {}

        void resetcurrenttime( int &currenttime ) {
            currenttime = list[0].arrival ;
            int index = 0 ;
            while ( index < list.size()) {
                if ( currenttime > list[ index ].arrival ) {
                    currenttime = list[ index ].arrival ;
                } // if
                index ++ ;
            } // while
        } // resetcurrenttime

        void setreadyqueue( int time ) {
            // cout << "time" << time  << endl ;
            int listindex = 0 ;
            Job job ;
            while( listindex < list.size() ) {
                if ( list[ listindex ].arrival <= time ) {
                    job.pid = list[ listindex ].pid ;
                    job.cpuburst = list[ listindex ].cpuburst ;
                    job.arrival = list[ listindex ].arrival ;
                    job.priority = list[ listindex ].priority ;
                    readyqueue.push_back( job ) ;
                    list.erase( list.begin() + listindex ) ;
                    listindex -= 1 ;
                } // if 
                listindex += 1 ;
            } // while
            // cout << list.size() << endl ;
        } // setreadyqueue

        virtual void dispatch( int &currenttime ) {
            int readyindex = 0 ; 
            int targetindex = 0 ;
            Job target = readyqueue[ 0 ] ;
            while ( readyindex < readyqueue.size() ) {
                if (  readyqueue[ readyindex ].arrival < target.arrival ) {
                    targetindex = readyindex ;
                    target = readyqueue[ readyindex ] ;
                } // if
                else if ( readyqueue[ readyindex ].arrival == target.arrival ) {
                    if ( readyqueue[ readyindex ].pid < target.pid ) {
                        targetindex = readyindex ;
                        target = readyqueue[ readyindex ];
                    } // if
                } // else if
                readyindex += 1 ;
            } // while

            readyqueue.erase( readyqueue.begin() + targetindex ) ;
            currenttime += target.cpuburst ;
            Finishjob finish ;
            finish.pid = target.pid ;
            finish.arrival = target.arrival ;
            finish.finishtime = currenttime ;
            finish.turnaroundtime = currenttime - target.arrival ;
            finish.waittime = finish.turnaroundtime - target.cpuburst ;
            finishlist.push_back( finish ) ;
        } // dispatch

        void schelduing() {
            Job firstjob = list[0] ;
            int currenttime = -1 ;
            resetcurrenttime( currenttime ) ;
            bool allfinish = false ;
            while  ( allfinish != true ) {
                // cout << "current" << currenttime << endl ;
                setreadyqueue( currenttime ) ;
                if ( readyqueue.empty() && list.empty() != true ) {
                    resetcurrenttime( currenttime ) ;
                } // if
                else {
                    dispatch( currenttime ) ;
                } // else

                if ( readyqueue.empty() && list.empty() ) {
                    allfinish = true ;
                } // if
                
            } // while
            
            // int i = 0 ;
            // while( i < finishlist.size() ) {
            //     cout << finishlist[i].pid << " " << finishlist[i].finishtime << endl ;
            //     i++ ;
            // } // while
        } // schelding


        void setgantt() {
            int time = 0 ;
            int index = 0 ;
            while ( index < finishlist.size() ) {
                if ( finishlist[ index ].arrival > time ) {
                    ganttstr += "-" ;
                } // if 
                else if( finishlist[ index ].finishtime > time ) {
                    ganttstr += gtf.pidtransfer( finishlist[ index ].pid ) ;
                } // else
                else if ( finishlist[ index ].finishtime <= time ) {
                    index++ ;
                    time-- ;
                } // else if
                time++ ;
            } // while
            cout << "gantt : " << endl ;
            cout << ganttstr << endl ;
        } // setgantt

        void sortfinishlist() {
            for ( int i = 0 ; i < finishlist.size() ; i ++ ) {
                for ( int n = i + 1; n < finishlist.size() ; n++ ) {
                    if ( finishlist[ i ].pid > finishlist [ n ].pid ) {
                        Finishjob temp = finishlist[ i ] ;
                        finishlist[ i ] = finishlist[ n ] ;
                        finishlist[ n ] = temp ;
                    } // if
                } // for
            } // for
        } // sortfinishlist 

        virtual void writefile( string inputstr ) {
            string outputfilename = "" ;
            outputfilename += inputstr ;
            outputfilename += "_method1.txt" ;
            ofstream outputfile ;
            outputfile.open( outputfilename ) ;
            if ( outputfile.is_open() ) {
                outputfile << "FCFS\n" ;
                outputfile << "==        FCFS==\n" ;
                outputfile << ganttstr + "\n" ;
                outputfile << "===========================================================\n\n" ;
                outputfile << "Waiting Time\n" ;
                outputfile << "ID\tFCFS\n" ;
                outputfile << "===========================================================\n" ;
                int index = 0 ;
                while ( index < finishlist.size() ) {
                    outputfile << finishlist[ index ].pid ;
                    outputfile << "\t" ;
                    outputfile << finishlist[ index ].waittime ;
                    outputfile << "\n" ;
                    index ++ ;
                } // while
                outputfile << "===========================================================\n\n" ;
                outputfile << "Turnaround Time\n" ;
                outputfile << "ID	FCFS\n" ;
                outputfile << "===========================================================\n" ;
                index = 0 ;
                while ( index < finishlist.size() ) {
                    outputfile << finishlist[ index ].pid ;
                    outputfile << "\t" ;
                    outputfile << finishlist[ index ].turnaroundtime ;
                    outputfile << "\n" ;
                    index ++ ;
                } // while
                outputfile << "===========================================================\n" ;
                cout << "write " << outputfilename << " successful " << endl ;
                outputfile.close() ;
            } // if
            else {
                cout << "error for write file " << endl ;
            } // else 
        } // writefile
} ; // FCFS

class SJF : public FCFS {
    public : 
        SJF( ) { }
        ~SJF( ) { }

        void dispatch( int &currenttime ) override {
            int readyindex = 0 ; 
            int targetindex = 0 ;
            Job target = readyqueue[ 0 ] ;
            while ( readyindex < readyqueue.size() ) {
                if (  readyqueue[ readyindex ].cpuburst < target.cpuburst ) {
                    targetindex = readyindex ;
                    target = readyqueue[ readyindex ];
                } // if
                else if (  readyqueue[ readyindex ].cpuburst == target.cpuburst ) {
                     if (  readyqueue[ readyindex ].arrival < target.arrival ) {
                        targetindex = readyindex ;
                        target = readyqueue[ readyindex ];
                     } // if
                     else if ( readyqueue[ readyindex ].arrival == target.arrival ) {
                        if ( readyqueue[ readyindex ].pid < target.pid ) {
                            targetindex = readyindex ;
                            target = readyqueue[ readyindex ] ;
                        } // if
                     } // else if
                } // else if
                readyindex += 1 ;
            } // while

           
            readyqueue.erase( readyqueue.begin() + targetindex ) ;
            currenttime += target.cpuburst ;
            Finishjob finish ;
            finish.finishtime = currenttime ;
            finish.arrival = target.arrival ;
            finish.turnaroundtime = finish.finishtime - target.arrival ;
            finish.pid = target.pid ;
            finish.waittime = finish.turnaroundtime - target.cpuburst ;
            finishlist.push_back( finish ) ;
        } // dispatch

        void writefile( string inputstr ) override {
            string outputfilename = "" ;
            outputfilename += inputstr ;
            outputfilename += "_method3.txt" ;
            ofstream outputfile ;
            outputfile.open( outputfilename ) ;
            if ( outputfile.is_open() ) {
                outputfile << "SJF\n" ;
                outputfile << "==        SJF==\n" ;
                outputfile << ganttstr + "\n" ;
                outputfile << "===========================================================\n\n" ;
                outputfile << "Waiting Time\n" ;
                outputfile << "ID\tSJF\n" ;
                outputfile << "===========================================================\n" ;
                int index = 0 ;
                while ( index < finishlist.size() ) {
                    outputfile << finishlist[ index ].pid ;
                    outputfile << "\t" ;
                    outputfile << finishlist[ index ].waittime ;
                    outputfile << "\n" ;
                    index ++ ;
                } // while
                outputfile << "===========================================================\n\n" ;
                outputfile << "Turnaround Time\n" ;
                outputfile << "ID	SJF\n" ;
                outputfile << "===========================================================\n" ;
                index = 0 ;
                while ( index < finishlist.size() ) {
                    outputfile << finishlist[ index ].pid ;
                    outputfile << "\t" ;
                    outputfile << finishlist[ index ].turnaroundtime ;
                    outputfile << "\n" ;
                    index ++ ;
                } // while
                outputfile << "===========================================================\n" ;
                cout << "write " << outputfilename << " successful " << endl ;
                outputfile.close() ;
            } // if
            else {
                cout << "error for write file " << endl ;
            } // else 
        } // writefile
} ; // SJF

class HRRN : public FCFS {
    public : 
        HRRN( ) { }
        ~HRRN( ) { }

        void dispatch( int &currenttime ) {
            int readyindex = 0 ; 
            int targetindex = 0 ;
            Job target = readyqueue[ 0 ] ;
            double targetratio = 0 ;
            if ( currenttime - readyqueue[readyindex].arrival > 0 )
                targetratio = ( double ) ( target.cpuburst + ( currenttime - target.arrival ) ) / ( double ) target.cpuburst ;
            else 
                targetratio = ( double ) ( target.cpuburst ) / ( double ) target.cpuburst ;

            while ( readyindex < readyqueue.size() ) {
                double ratio = 0 ;
                if ( currenttime - readyqueue[readyindex].arrival > 0 )
                    ratio = ( double ) ( readyqueue[ readyindex ].cpuburst + ( currenttime - readyqueue[readyindex].arrival ) ) /  ( double ) readyqueue[ readyindex ].cpuburst ;
                else 
                    ratio = ( double ) ( readyqueue[ readyindex ].cpuburst ) / ( double ) readyqueue[ readyindex ].cpuburst ;

                if (  ratio > targetratio ) {
                    targetindex = readyindex ;
                    target = readyqueue[ readyindex ] ;
                    targetratio = ratio ;
                } // if
                else if ( ratio == targetratio ) {
                    if ( readyqueue[ readyindex ].arrival < target.arrival ) {
                        targetindex = readyindex ;
                        target = readyqueue[ readyindex ];
                        targetratio = ratio ;
                    } // if
                    else if ( readyqueue[ readyindex ].arrival == target.arrival ) {
                        if ( readyqueue[ readyindex ].pid < target.pid ) {
                            targetindex = readyindex ;
                            target = readyqueue[ readyindex ];
                            targetratio = ratio ;
                        } // if
                    } // else if
                } // else if
                readyindex += 1 ;
            } // while

            readyqueue.erase( readyqueue.begin() + targetindex ) ;
            currenttime += target.cpuburst ;
            Finishjob finish ;
            finish.finishtime = currenttime ;
            finish.pid = target.pid ;
            finish.arrival = target.arrival ;
            finish.turnaroundtime = finish.finishtime - target.arrival ;
            finish.waittime = finish.turnaroundtime - target.cpuburst ;
            finishlist.push_back( finish ) ;
        } // dispatch

        void writefile( string inputstr ) override {
            string outputfilename = "" ;
            outputfilename += inputstr ;
            outputfilename += "_method5.txt" ;
            ofstream outputfile ;
            outputfile.open( outputfilename ) ;
            if ( outputfile.is_open() ) {
                outputfile << "HRRN\n" ;
                outputfile << "==        HRRN==\n" ;
                outputfile << ganttstr + "\n" ;
                outputfile << "===========================================================\n\n" ;
                outputfile << "Waiting Time\n" ;
                outputfile << "ID\tHRRN\n" ;
                outputfile << "===========================================================\n" ;
                int index = 0 ;
                while ( index < finishlist.size() ) {
                    outputfile << finishlist[ index ].pid ;
                    outputfile << "\t" ;
                    outputfile << finishlist[ index ].waittime ;
                    outputfile << "\n" ;
                    index ++ ;
                } // while
                outputfile << "===========================================================\n\n" ;
                outputfile << "Turnaround Time\n" ;
                outputfile << "ID	HRRN\n" ;
                outputfile << "===========================================================\n" ;
                index = 0 ;
                while ( index < finishlist.size() ) {
                    outputfile << finishlist[ index ].pid ;
                    outputfile << "\t" ;
                    outputfile << finishlist[ index ].turnaroundtime ;
                    outputfile << "\n" ;
                    index ++ ;
                } // while
                outputfile << "===========================================================\n" ;
                cout << "write " << outputfilename << " successful " << endl ;
                outputfile.close() ;
            } // if
            else {
                cout << "error for write file " << endl ;
            } // else 
        } // writefile
}  ; // HRRN

struct Jobver2 {
    int pid ;
    int arrival ;
    int cpuburst ;
    int remindcpuburst ;
    int priority ;
    int nexttime ;
} ;

class RR {
    public :
        int timeslice ;
        vector < Jobver2 > list ;
        vector < Jobver2 > readyqueue ;
        vector < Finishjob > finishlist ;
        vector < Finishjob > ganttchart ;
        string ganttstr ;
        Ganttpidtranfer gtf ;
        Jobver2 lastdispatchjob ;

        void setJoblist( vector <Job> joblist ) { 
            int i = 0 ;
            Jobver2 job ;
            while ( i < joblist.size() ) {
                job.pid = joblist[i].pid ;
                job.arrival = joblist[i].arrival ;
                job.cpuburst = joblist[i].cpuburst ;
                job.priority = joblist[i].priority ;
                job.remindcpuburst = joblist[i].cpuburst ;
                job.nexttime = 0 ;
                list.push_back( job ) ;
                i ++ ;
            } // while

        } // setJoblist
        RR( ) { ganttstr = "" ; }
        ~RR() {}

        void settimeslice( int slice ) {
            timeslice = slice ;
        } // setter

        void resetcurrenttime( int &currenttime ) {
            currenttime = list[0].arrival ;
            int index = 0 ;
            while ( index < list.size()) {
                if ( currenttime > list[ index ].arrival ) {
                    currenttime = list[ index ].arrival ;
                } // if
                index ++ ;
            } // while
        } // resetcurrenttime

        void sortforarrivalandpid( vector <Jobver2> &list ) {
            Jobver2 temp ;
            for ( int i = 0 ; i < list.size() ; i ++ ) {
                for ( int j = i + 1; j < list.size() ; j++ ) {
                    if ( list[ i ].arrival > list[ j ].arrival ) {
                        temp = list[i] ;
                        list[i] = list[j] ;
                        list[j] = temp ;
                    } // if
                    else if ( list[ i ].arrival == list[ j ].arrival ) {
                        if ( list[ i ].pid > list[ j ].pid ) {
                            temp = list[i] ;
                            list[i] = list[j] ;
                            list[j] = temp ;
                        } // if
                    } // else if
                } // for
            } // for
        } // sortforarrivalandpid

        void setreadyqueue( int time, bool istimeout, bool setdispatch ) {
            // cout << "time" << time  << endl ;
            int listindex = 0 ;
            Jobver2 job ;
            vector <Jobver2> isfitlist ;
            vector <Jobver2> arrivalsametimeout ;
            while( listindex < list.size() ) {
                if ( istimeout == false ) {
                    if ( list[ listindex ].arrival <= time ) {
                            job = list[ listindex ] ;
                            isfitlist.push_back( job ) ;
                            list.erase( list.begin() + listindex ) ;
                            listindex -= 1 ;
                    } // if
                } // if
                else { //istimeout = true
                    if ( list[ listindex ].arrival < time ) {
                        job = list[ listindex ] ;
                        isfitlist.push_back( job ) ;
                        list.erase( list.begin() + listindex ) ;
                        listindex -= 1 ;
                    } // if
                    else if ( list[ listindex ].arrival == time ) {
                        job = list[ listindex ] ;
                        arrivalsametimeout.push_back( job ) ;
                        list.erase( list.begin() + listindex ) ;
                        listindex -= 1 ;
                    } // else if 
                }  // else 
                listindex += 1 ;
            } // while

            sortforarrivalandpid( isfitlist ) ;
            sortforarrivalandpid( arrivalsametimeout ) ;
            // isfitlist sort for arrival ;
            int index = 0 ;


            index = 0 ;
            while ( index < isfitlist.size() ) {
                readyqueue.push_back( isfitlist[ index ] ) ;
                index ++ ;
            } // while

            // if ( setdispatch == true ) {
               // deal lastdispatch 
                // index = 0 ;
                // bool isin = false ;
                // while ( index < arrivalsametimeout.size() ) {
                //     if ( arrivalsametimeout[ index ].arrival > lastdispatchjob.nexttime ) {
                //         if ( arrivalsametimeout[ index ].pid > lastdispatchjob.pid ) {
                //             arrivalsametimeout.insert( arrivalsametimeout.begin() + index, lastdispatchjob ) ;
                //             isin = true ;
                //             index = arrivalsametimeout.size() ;
                //         } // if
                //     } // if
                //     index ++ ;
                // } // while 
                // if (  )
                // readyqueue.push_back( lastdispatchjob ) ;
            // } // if

            index = 0 ;
            while ( index < arrivalsametimeout.size() ) {
                readyqueue.push_back( arrivalsametimeout[ index ] ) ;
                index ++ ;
            } // while
            if ( setdispatch == true )
                readyqueue.push_back( lastdispatchjob ) ;

            // index = 0 ;
            // cout << "readyqueue : " ;
            // while ( index < readyqueue.size() ) {
            //     cout << readyqueue[ index ].pid << " " ;
            //     index ++ ;
            // } // while
            // cout << endl ;
        } // setreadyqueue

        virtual void dispatch( int &currenttime, int timeslice, bool &istimeout ,bool &setdispatch, bool &stillrun ) {
            setdispatch = false ;
            istimeout = false ;
            lastdispatchjob = readyqueue[0] ;
            // cout << "dispatch : " << lastdispatchjob.pid << " cpu :" << lastdispatchjob.remindcpuburst << endl ;
            readyqueue.erase( readyqueue.begin() ) ;
            if ( lastdispatchjob.remindcpuburst > timeslice ) {
                // cout << "this dispatch need nect tuen " << endl ;
                lastdispatchjob.remindcpuburst -= timeslice ;
                currenttime += timeslice ;
                lastdispatchjob.nexttime = currenttime  ;
                Finishjob ganttpart ;
                ganttpart.arrival = lastdispatchjob.arrival ;
                ganttpart.pid = lastdispatchjob.pid ;
                ganttpart.finishtime = currenttime ;
                ganttchart.push_back( ganttpart ) ;
                setdispatch = true ;
                istimeout = true ;
                stillrun = true ;
            } // if
            else { // ( lastdispatchjob.cpuburst <= timeslice ) // finish
                currenttime += lastdispatchjob.remindcpuburst ;
                Finishjob ganttpart ;
                Finishjob finish ;
                finish.arrival = lastdispatchjob.arrival ;
                finish.pid = lastdispatchjob.pid ;
                finish.finishtime = currenttime ;
                finish.turnaroundtime = currenttime - finish.arrival ;
                finish.waittime = finish.turnaroundtime - lastdispatchjob.cpuburst ;

                ganttpart.pid = lastdispatchjob.pid ;
                ganttpart.arrival = lastdispatchjob.arrival ;
                ganttpart.finishtime = currenttime ;
                // int turnaroundtime = currenttime - lastdispatchjob.arrival ;
                // ganttpart.waittime = turnaroundtime - lastdispatchjob.cpuburst ;
                finishlist.push_back( finish ) ;
                ganttchart.push_back( ganttpart ) ;
                stillrun = false ;
            } // else 
            
        } // dispatch

        void schelduing() {
            Jobver2 firstjob = list[0] ;
            int currenttime = -1 ;
            resetcurrenttime( currenttime ) ;
            bool allfinish = false, istimeoout = false, setdispatch = false ; 
            bool stillrun = false ;
            while  ( allfinish != true ) {
                // cout << "current : " << currenttime << endl ;
                setreadyqueue( currenttime, istimeoout, setdispatch ) ;
                if ( readyqueue.empty() && list.empty() != true ) {
                    // cout << "reset time " << endl ;
                    resetcurrenttime( currenttime ) ;
                } // if
                else {
                    dispatch( currenttime, timeslice, istimeoout, setdispatch, stillrun ) ;
                } // else

                if ( readyqueue.empty() && list.empty() && stillrun == false  ) {
                    allfinish = true ;
                } // if
                // cout << " ============= next tuen ======== " << endl ;
            } // while
            
            // int i = 0 ;
            // while( i < finishlist.size() && i < 5 ) {
            //     cout << finishlist[i].pid << " " << finishlist[i].finishtime << endl ;
            //     i++ ;
            // } // while

        } // schelding

        void printganttchar() {
            int index = 0 ;
            while ( index < ganttchart.size() ) {
                cout << ganttchart[ index ].pid << " " ;
                index ++ ;
            } // while
            cout << endl ;
        } // void

        void sortfinishlist() {
            for ( int i = 0 ; i < finishlist.size() ; i ++ ) {
                for ( int n = i + 1; n < finishlist.size() ; n++ ) {
                    if ( finishlist[ i ].pid > finishlist [ n ].pid ) {
                        Finishjob temp = finishlist[ i ] ;
                        finishlist[ i ] = finishlist[ n ] ;
                        finishlist[ n ] = temp ;
                    } // if
                } // for
            } // for
        } // sortfinishlist 

        void setgantt() {
            int index = 0 ;
            int time = 0 ;
            while ( index < ganttchart.size() ) {
                if ( ganttchart[ index ].arrival > time ) {
                    ganttstr += "-" ;
                } // if 
                else if( ganttchart[ index ].finishtime > time ) {
                    ganttstr += gtf.pidtransfer( ganttchart[ index ].pid ) ;
                } // else
                else if ( ganttchart[ index ].finishtime <= time ) {
                    index++ ;
                    time-- ;
                } // else if
                time++ ;
            } // while
            cout << "gantt : " << endl ;
            cout << ganttstr << endl ;
        } // setgantt

        void writefile( string inputstr ) {
            string outputfilename = "" ;
            outputfilename += inputstr ;
            outputfilename += "_method2.txt" ;
            ofstream outputfile ;
            outputfile.open( outputfilename ) ;
            if ( outputfile.is_open() ) {
                outputfile << "RR\n" ;
                outputfile << "==        RR==\n" ;
                outputfile << ganttstr + "\n" ;
                outputfile << "===========================================================\n\n" ;
                outputfile << "Waiting Time\n" ;
                outputfile << "ID\tRR\n" ;
                outputfile << "===========================================================\n" ;
                int index = 0 ;
                while ( index < finishlist.size() ) {
                    outputfile << finishlist[ index ].pid ;
                    outputfile << "\t" ;
                    outputfile << finishlist[ index ].waittime ;
                    outputfile << "\n" ;
                    index ++ ;
                } // while
                outputfile << "===========================================================\n\n" ;
                outputfile << "Turnaround Time\n" ;
                outputfile << "ID	RR\n" ;
                outputfile << "===========================================================\n" ;
                index = 0 ;
                while ( index < finishlist.size() ) {
                    outputfile << finishlist[ index ].pid ;
                    outputfile << "\t" ;
                    outputfile << finishlist[ index ].turnaroundtime ;
                    outputfile << "\n" ;
                    index ++ ;
                } // while
                outputfile << "===========================================================\n" ;
                cout << "write " << outputfilename << " successful " << endl ;
                outputfile.close() ;
            } // if
            else {
                cout << "error for write file " << endl ;
            } // else 
        } // writefile
} ; // RR

class SRTF {
    public :
        Ganttpidtranfer gtf ;
        string ganttstr ;
        vector < Jobver2 > list ;
        vector < Jobver2 > readyqueue ;
        vector < Finishjob > ganttchart ;
        vector < Finishjob > finishlist ;
        int lastcurrenttime ;
        int lasttargetpid ;
        void setJoblist( vector <Job> joblist ) { 
            int i = 0 ;
            Jobver2 job ;
            while ( i < joblist.size() ) {
                job.pid = joblist[i].pid ;
                job.arrival = joblist[i].arrival ;
                job.cpuburst = joblist[i].cpuburst ;
                job.priority = joblist[i].priority ;
                job.remindcpuburst = joblist[i].cpuburst ;
                job.nexttime = 0 ;
                list.push_back( job ) ;
                i ++ ;
            } // while

        } // setJoblist
        
        SRTF() { 
            lastcurrenttime = -1 ; 
            ganttstr = "" ;
        } // constructor
        ~SRTF() {}

        void setcurrenttime( int &currenttime ) {
            int index = 0 ;
            int minarrival = -1 ;
            while ( index < list.size()) {
                if ( minarrival > list[ index ].arrival || minarrival == -1 ) {
                    minarrival = list[ index ].arrival ;
                } // if
                index ++ ;
            } // while
           
            if ( minarrival != -1 ) {
                currenttime = minarrival ;
            } // if

            if ( lastcurrenttime != -1 && readyqueue.size() != 0 ) {
                int dispatchindex = -1 ;
                index = 0 ;
                while ( index < readyqueue.size() ) {
                    if ( readyqueue[ index ].pid == lasttargetpid ) {
                        dispatchindex = index ;
                        index = readyqueue.size() ;
                    } // if
                    index ++ ;
                } // while
                
                if ( minarrival == -1 ) {
                    currenttime = lastcurrenttime + readyqueue[ dispatchindex ].remindcpuburst ;
                } // if
                else if ( lastcurrenttime + readyqueue[ dispatchindex ].remindcpuburst < currenttime ) {
                    currenttime = lastcurrenttime + readyqueue[ dispatchindex ].remindcpuburst ;
                } // else if
            } // if
        } // setcurrenttime

        void setreadyqueue( int time ) {
            // cout << "time" << time  << endl ;
            int listindex = 0 ;
            Jobver2 job ;
            while( listindex < list.size() ) {
                if ( list[ listindex ].arrival <= time ) {
                    job = list[ listindex ] ;
                    readyqueue.push_back( job ) ;
                    list.erase( list.begin() + listindex ) ;
                    listindex -= 1 ;
                } // if 
                listindex += 1 ;
            } // while
            // cout << list.size() << endl ;

        } // setreadyqueue

        virtual void dispatch( int &currenttime, bool& isidle ) {
            int readyindex = 1 ; 
            int targetindex = -1 ;        
            int lasttarget = -1 ;   
            int index = 0 ;
            if ( lastcurrenttime != -1 && lasttargetpid != -1 ) {
                while ( index < readyqueue.size() ) {
                    if ( readyqueue[ index ].pid == lasttargetpid ) {
                        lasttarget = index ;
                        index = readyqueue.size() ;
                    } // if 
                    index ++ ;
                }  // while
                
                // cout << "lasttarget : " << readyqueue[ lasttarget ].pid << " reburst : " << readyqueue[ lasttarget ].remindcpuburst  << endl ;
                if ( readyqueue[ lasttarget ].remindcpuburst > ( currenttime - lastcurrenttime ) ) {
                    readyqueue[ lasttarget ].remindcpuburst = readyqueue[ lasttarget ].remindcpuburst - ( currenttime - lastcurrenttime ) ;
                    // cout << "whole time is do this job " << readyqueue[ lasttarget ].pid << endl ;
                    Finishjob ganttpart ;
                    ganttpart.pid = readyqueue[ lasttarget ].pid ;
                    ganttpart.arrival = readyqueue[ lasttarget ].arrival ;
                    ganttpart.finishtime = currenttime ;
                    ganttchart.push_back( ganttpart ) ;
                    
                } // if 
                else { // <=       
                    // currenttime = lastcurrenttime + readyqueue[ lasttarget ].remindcpuburst ;
                    Finishjob finish ;
                    Finishjob ganttpart ;
                    finish.pid = readyqueue[ lasttarget ].pid ;
                    finish.arrival = readyqueue[ lasttarget ].arrival ;
                    finish.turnaroundtime = currenttime - finish.arrival ;
                    finish.waittime = finish.turnaroundtime - readyqueue[ lasttarget ].cpuburst ;

                    ganttpart.pid = readyqueue[ lasttarget ].pid ;
                    ganttpart.arrival = readyqueue[ lasttarget ].arrival ;
                    ganttpart.finishtime = currenttime ;
                    finishlist.push_back( finish ) ;
                    ganttchart.push_back( ganttpart ) ;
                    readyqueue.erase( readyqueue.begin() + lasttarget ) ;
                    if ( readyqueue.size()  == 0 ) {
                        // cout << " next idle" << endl ;
                        isidle = true ;
                        lasttargetpid = -1 ;
                    } // if
                } // else
                
            } // if

            // index = 0 ;
            // cout << "readyqueue : " ;
            // while ( index < readyqueue.size() ) {
            //     cout << readyqueue[ index ].pid << " " ;
            //     index++ ;
            // } // while
            // cout << endl ;

            if ( isidle != true ) {
                Jobver2 target = readyqueue[0]  ;
                while ( readyindex < readyqueue.size() ) {
                    // cout << "  " << readyqueue[ readyindex ].remindcpuburst << "     " << target.remindcpuburst << endl ;
                    if ( readyqueue[ readyindex ].remindcpuburst < target.remindcpuburst ) {
                        targetindex = readyindex ;
                        target = readyqueue[ readyindex ] ;
                    } // if
                    else if (  readyqueue[ readyindex].remindcpuburst == target.remindcpuburst ) {
                        if (  readyqueue[ readyindex].arrival < target.arrival  ) {
                            targetindex = readyindex ;
                            target = readyqueue[ readyindex ] ;
                        } // if
                        else if ( readyqueue[ readyindex].arrival == target.arrival  ) {
                            if ( readyqueue[ readyindex].pid < target.pid ) {
                                targetindex = readyindex ;
                                target = readyqueue[ readyindex ] ;
                            } // if
                        } // else if
                    } // else if 
                    readyindex += 1 ;
                } // while

                // cout << "dispatch " << target.pid << endl ; 
                lastcurrenttime = currenttime ;
                lasttargetpid = target.pid ;
            } // if

            isidle = false ;
            
        } // dispatch

        void schelduing() {
            int currenttime = -1 ;
            setcurrenttime( currenttime ) ;
            bool allfinish = false ;
            bool isidle = false ;
            while  ( allfinish != true ) {
               
                // cout << "current : " << currenttime << endl ;
                setreadyqueue( currenttime ) ;
                dispatch( currenttime, isidle ) ;
                
                if ( readyqueue.empty() && list.empty() ) {
                    allfinish = true ;
                } // if
                else {
                    setcurrenttime( currenttime ) ;
                } // else 
                // cout << "next turn " << endl << endl  ;
            } // while
        } // schelding

        void printganttchar() {
            int index = 0 ;
            while ( index < ganttchart.size() ) {
                cout << ganttchart[ index ].pid << " " ;
                index ++ ;
            } // while
            cout << endl ;
        } // void

        void sortfinishlist() {
            for ( int i = 0 ; i < finishlist.size() ; i ++ ) {
                for ( int n = i + 1; n < finishlist.size() ; n++ ) {
                    if ( finishlist[ i ].pid > finishlist [ n ].pid ) {
                        Finishjob temp = finishlist[ i ] ;
                        finishlist[ i ] = finishlist[ n ] ;
                        finishlist[ n ] = temp ;
                    } // if
                } // for
            } // for
        } // sortfinishlist

        void setgantt() {
            int index = 0 ;
            int time = 0 ;
            while ( index < ganttchart.size() ) {
                if ( ganttchart[ index ].arrival > time ) {
                    ganttstr += "-" ;
                } // if 
                else if( ganttchart[ index ].finishtime > time ) {
                    ganttstr += gtf.pidtransfer( ganttchart[ index ].pid ) ;
                } // else
                else if ( ganttchart[ index ].finishtime <= time ) {
                    index++ ;
                    time-- ;
                } // else if
                time++ ;
            } // while
            cout << "gantt : " << endl ;
            cout << ganttstr << endl ;
        } // setgantt

        void writefile( string inputstr ) {
            string outputfilename = "" ;
            outputfilename += inputstr ;
            outputfilename += "_method4.txt" ;
            ofstream outputfile ;
            outputfile.open( outputfilename ) ;
            if ( outputfile.is_open() ) {
                outputfile << "SRJF\n" ;
                outputfile << "==        SRJF==\n" ;
                outputfile << ganttstr + "\n" ;
                outputfile << "===========================================================\n\n" ;
                outputfile << "Waiting Time\n" ;
                outputfile << "ID\tSRJF\n" ;
                outputfile << "===========================================================\n" ;
                int index = 0 ;
                while ( index < finishlist.size() ) {
                    outputfile << finishlist[ index ].pid ;
                    outputfile << "\t" ;
                    outputfile << finishlist[ index ].waittime ;
                    outputfile << "\n" ;
                    index ++ ;
                } // while
                outputfile << "===========================================================\n\n" ;
                outputfile << "Turnaround Time\n" ;
                outputfile << "ID	SRJF\n" ;
                outputfile << "===========================================================\n" ;
                index = 0 ;
                while ( index < finishlist.size() ) {
                    outputfile << finishlist[ index ].pid ;
                    outputfile << "\t" ;
                    outputfile << finishlist[ index ].turnaroundtime ;
                    outputfile << "\n" ;
                    index ++ ;
                } // while
                outputfile << "===========================================================\n" ;
                cout << "write " << outputfilename << " successful " << endl ;
                outputfile.close() ;
            } // if
            else {
                cout << "error for write file " << endl ;
            } // else 
        } // writefile
} ; // SRTF

class PPRR {
    public :
        Ganttpidtranfer gtf ;
        string ganttstr ;
        int timeslice ;
        vector < Jobver2 > list ;
        vector < Jobver2 > readyqueue ;
        vector < Finishjob > finishlist ;
        vector < Finishjob > ganttchart ;
        Jobver2 lastdispatchjob ;

        void setJoblist( vector <Job> joblist ) { 
            int i = 0 ;
            Jobver2 job ;
            while ( i < joblist.size() ) {
                job.pid = joblist[i].pid ;
                job.arrival = joblist[i].arrival ;
                job.cpuburst = joblist[i].cpuburst ;
                job.priority = joblist[i].priority ;
                job.remindcpuburst = joblist[i].cpuburst ;
                job.nexttime = 0 ;
                list.push_back( job ) ;
                i ++ ;
            } // while

        } // setJoblist
        PPRR( ) { 
            lastdispatchjob.pid = -999 ;
            lastdispatchjob.arrival = -999 ;
            lastdispatchjob.cpuburst = -999 ;
            lastdispatchjob.nexttime = -999 ;
            lastdispatchjob.remindcpuburst = -999 ;
            ganttstr = "" ;
        } // construct
        ~PPRR() {}

        void settimeslice( int slice ) {
            timeslice = slice ;
        } // setter

        void sortforpriority( vector <Jobver2> &list ) {
            Jobver2 temp ;
            for ( int i = 0 ; i < list.size() ; i ++ ) {
                for ( int j = i + 1; j < list.size() ; j++ ) {
                    if ( list[i].priority > list[j].priority ) {
                        temp = list[i] ;
                        list[i] = list[j] ;
                        list[j] = temp ;
                    } // if 
                    else if ( list[i].priority == list[j].priority  ) {
                        if ( list[ i ].arrival > list[ j ].arrival ) {
                            temp = list[i] ;
                            list[i] = list[j] ;
                            list[j] = temp ;
                        } // if
                        else if ( list[ i ].arrival == list[ j ].arrival ) {
                            if ( list[ i ].pid > list[ j ].pid ) {
                                temp = list[i] ;
                                list[i] = list[j] ;
                                list[j] = temp ;
                            } // if
                        } // else if
                    } // else if
                } // for
            } // for
        } // sortforpriority

        
        void arrivalnodeinreadyqueue( Jobver2 job ) {
            int index = 0 ;
            bool isin = false ;
            while ( index < readyqueue.size() ) {
                if ( readyqueue[ index ].priority == job.priority ) {
                    readyqueue.insert( readyqueue.begin() + index , job ) ;
                    isin = true ;
                    index = readyqueue.size() ;
                } // if
                index ++ ;
            } // while

            if ( isin == false ) {
                index = 0 ;
                while ( index < readyqueue.size() ) {
                    if ( readyqueue[ index ].priority > job.priority ) {
                        readyqueue.insert( readyqueue.begin() + index , job ) ;
                        isin = true ;
                        index = readyqueue.size() ;
                    } // if
                    index ++ ;
                } // while
            } // if

            if ( isin == false ) {
                index = 0 ;
                readyqueue.push_back( job ) ;
            } // if

        } // arrivalnodeinreadyqueue


        void arrivaltoreadyqueue( vector<Jobver2> listb ) {
            Jobver2 job ;
            int listbindex = listb.size() - 1;            
            while ( listbindex > -1 ) {
                arrivalnodeinreadyqueue( listb[ listbindex ] ) ;
                listbindex-- ;
            } // while
        } // arrivaltoreadyqueue


        void isfitnodeinreadtqueue( Jobver2 job ) {
            int index = 0 ;
            bool isin = false ;
            while ( index < readyqueue.size() ) {
                if ( readyqueue[ index ].priority > job.priority ) {
                    readyqueue.insert( readyqueue.begin() + index , job ) ;
                    isin = true ;
                    index = readyqueue.size() ;
                } // if
                index ++ ;
            } // while

            if ( isin == false ) {
                index = 0 ;
                if (  readyqueue.size() != 0 && readyqueue[0].priority > job.priority ) {
                    readyqueue.insert( readyqueue.begin() , job ) ;
                    isin = true ; 
                } // if
            } // if

            if ( isin == false ) {
                index = 0 ;
                readyqueue.push_back( job ) ;
            } // if         

        }  // isfitnodeinreadtqueue

        void isfittoreadyqueue( vector <Jobver2> lista ) {
            Jobver2 job  ;
            int index = 0 ;
            while ( index < lista.size() ) {
                isfitnodeinreadtqueue( lista[ index ] ) ;
                index ++ ;
            } // while
        } // isfittoreadyqueue

        void setreadyqueue( int time, bool istimeout, bool &ispreemptive ) {
            ispreemptive = false ;
            vector <Jobver2> isfitlist ;
            int listindex = 0 ;
            Jobver2 job ;
            while ( listindex < list.size() ) {
                if ( list[ listindex ].arrival <= time ) {
                    job = list[ listindex ] ;
                    isfitlist.push_back( job ) ;
                } // if
                listindex ++ ;
            } // while
            
            if ( isfitlist.size() != 0 && lastdispatchjob.pid != -999 && lastdispatchjob.priority > isfitlist[0].priority ) {
                // cout << "preemptive " << endl ;
                ispreemptive = true ;
                // istimeout = true ;
            } // if

            if ( isfitlist.size() != 0 ) {
                listindex = 0 ;
                isfitlist.clear() ;
                vector <Jobver2> arrivalsametimeout ;
                while( listindex < list.size() ) {
                    if ( istimeout == false && ispreemptive == false ) {
                        if ( list[ listindex ].arrival <= time ) {
                                job = list[ listindex ] ;
                                isfitlist.push_back( job ) ;
                                list.erase( list.begin() + listindex ) ;
                                listindex -= 1 ;
                        } // if
                    } // if
                    else { //istimeout = true
                        if ( list[ listindex ].arrival < time ) {
                            job = list[ listindex ] ;
                            isfitlist.push_back( job ) ;
                            list.erase( list.begin() + listindex ) ;
                            listindex -= 1 ;
                        } // if
                        else if ( list[ listindex ].arrival == time ) {
                            job = list[ listindex ] ;
                            arrivalsametimeout.push_back( job ) ;
                            list.erase( list.begin() + listindex ) ;
                            listindex -= 1 ;
                        } // else if 
                    }  // else 
                    listindex += 1 ;
                } // while
                // cout << "finish list insert "<< endl ;
                sortforpriority( isfitlist ) ;
                sortforpriority( arrivalsametimeout ) ;
                // cout << "isfit : " << isfitlist.size() << " arrival : " << arrivalsametimeout.size() << endl  ;
                arrivaltoreadyqueue( arrivalsametimeout ) ;
                isfittoreadyqueue( isfitlist ) ;
            } // if
            // cout << "bb : " << readyqueue.size() ; 
            // index = 0 ;
            // cout << "readyqueue : " ;
            // while ( index < readyqueue.size() ) {
            //     cout << readyqueue[ index ].pid << " " ;
            //     index ++ ;
            // } // while
            // cout << endl ;
        } // setreadyqueue

        bool istheMaxinreadyqueue( Jobver2 job ) {
            int i = 0 ;
            // cout << "job" << job.priority << endl ;
            while ( i < readyqueue.size() ) {
                // cout << "p in r" << readyqueue[i].priority << " " ;
                if ( job.priority >= readyqueue[i].priority ) {
                    return false ;
                } // if
                i++ ;
            } // while
            return true ;
        } // void 

        void setRRjob() {
            // deal lastdispatch 
            int index = 0 ;
            bool isin = false ;
            while ( index < readyqueue.size() ) {
                if ( readyqueue[ index ].priority > lastdispatchjob.priority ) {
                    readyqueue.insert( readyqueue.begin() + index, lastdispatchjob ) ;
                    isin = true ;
                    index = readyqueue.size() ;
                } // if
                index ++ ;
            } // while 

            if ( isin == false ) {
                readyqueue.push_back( lastdispatchjob ) ;
            }  // if
        } // setRRjob


        virtual void dispatch( int currenttime, int &timeoutcount, bool &istimeout , bool ispreemptive) {
            if ( readyqueue.size() != 0 ) {
                if ( istimeout == true && readyqueue[0].pid == lastdispatchjob.pid && lastdispatchjob.pid != -999 ) {
                        // cout << " need rr" << endl ;
                        readyqueue.erase( readyqueue.begin() ) ;
                        // inert lastjob to priority in readyqueue ;
                        setRRjob() ;
                        istimeout = false ;
                    } // if
                    else {
                        istimeout = false ;
                    } // esle

                if ( ispreemptive ) {
                        int i = 0 ;
                        while ( i < readyqueue.size() ) {
                            if ( readyqueue[i].pid == lastdispatchjob.pid && lastdispatchjob.pid != -999 ) {
                                readyqueue.erase( readyqueue.begin() + i ) ;
                                i = readyqueue.size() ;
                            } // if
                            i++ ;
                        }  // while
                        setRRjob() ;
                    } // if

                if ( readyqueue[0].remindcpuburst > 1 ) {
                    if ( readyqueue[0].pid == lastdispatchjob.pid && lastdispatchjob.pid != -999 ) {
                        timeoutcount++ ;
                        // cout << "   timeoutcount : " << timeoutcount << endl ;
                    } // if
                    else {
                        timeoutcount = 1 ;
                        // cout << "reset timeout count" << endl ;
                    } // esle 

                    
                    if ( timeoutcount == timeslice && istheMaxinreadyqueue( readyqueue[0] ) != true ) {
                        // cout << "next must deal timeout " << endl ;
                        istimeout = true ;
                    } // if

                    // int index = 0 ;
                    // cout << "readyqueue : " ;
                    // while ( index < readyqueue.size() ) {
                    //     cout << readyqueue[ index ].pid << " " ;
                    //     index ++ ;
                    // } // while
                    // cout << endl ;

                    // cout << "  bbbb dispatch job :" << readyqueue[0].pid << " recpubrust : "  << readyqueue[0].remindcpuburst << endl ;
                    readyqueue[0].remindcpuburst -= 1 ;
                    // cout << "  dispatch job :" << readyqueue[0].pid << "recpubrust : "  << readyqueue[0].remindcpuburst << endl ;
                    Finishjob ganttpart ;
                    ganttpart.pid = readyqueue[0].pid ;
                    ganttpart.arrival = readyqueue[0].arrival ;
                    ganttpart.finishtime = currenttime + 1 ; //mean t ~  t+ 1 time
                    ganttchart.push_back( ganttpart ) ;
                    lastdispatchjob = readyqueue[0] ;
                } // if
                else { // finishjob  readyqueue[0].remindcpuburst == 1

                    istimeout = false ;                    
                    // cout << "this job is done : " << readyqueue[0].pid << endl ;
                    Finishjob finish ;
                    finish.pid = readyqueue[ 0 ].pid ;
                    finish.arrival = readyqueue[ 0 ].arrival ;
                    finish.finishtime = currenttime + 1 ;
                    finish.turnaroundtime = finish.finishtime - readyqueue[0].arrival ;
                    finish.waittime = finish.turnaroundtime - readyqueue[0].cpuburst ;
                    finishlist.push_back( finish ) ;
                    Finishjob ganttpart ;
                    ganttpart.pid = readyqueue[0].pid ;
                    ganttpart.arrival = readyqueue[0].arrival ;
                    ganttpart.finishtime = currenttime + 1 ; //mean t ~  t+ 1 time
                    ganttchart.push_back( ganttpart ) ;
                    readyqueue.erase( readyqueue.begin() ) ;
                } // else 
                
                
            } // if
            
                // cout << "idle" << endl ;

        } // dispatch

        void schelduing() {
            Jobver2 firstjob = list[0] ;
            int currenttime = 0  ;
            bool allfinish = false, istimeout = false, ispreemptive = false  ; 
            int timeoutcount = 1  ;
            int ii = 0 ; 
            while  ( allfinish != true ) {
                // cout << "current : " << currenttime << endl ;
                setreadyqueue( currenttime, istimeout, ispreemptive ) ;       
                dispatch( currenttime, timeoutcount, istimeout,ispreemptive ) ;
                if ( readyqueue.empty() && list.empty() ) {
                    allfinish = true ;
                } // if
                currenttime++;
                // cout << "    ============= next tuen ======== " << endl ;
                // if ( currenttime == 18 ) 
                    // allfinish = true ;
            } // while
            
        } // schelding

        void printganttchar() {
            int index = 0 ;
            while ( index < ganttchart.size() ) {
                cout << ganttchart[ index ].pid << " " ;
                index ++ ;
            } // while
            cout << endl ;
        } // void

        void sortfinishlist() {
            for ( int i = 0 ; i < finishlist.size() ; i ++ ) {
                for ( int n = i + 1; n < finishlist.size() ; n++ ) {
                    if ( finishlist[ i ].pid > finishlist [ n ].pid ) {
                        Finishjob temp = finishlist[ i ] ;
                        finishlist[ i ] = finishlist[ n ] ;
                        finishlist[ n ] = temp ;
                    } // if
                } // for
            } // for
        } // sortfinishlist

        void setgantt() {
            int index = 0 ;
            int time = 0 ;
            while ( index < ganttchart.size() ) {
                if ( ganttchart[ index ].arrival > time  ) {
                    ganttstr += "-" ;
                    index-- ;
                } // if 
                else {
                    ganttstr += gtf.pidtransfer( ganttchart[ index ].pid ) ;
                } // else
                index ++ ;
                time ++ ;
            } // while
            cout << "gantt : " << endl ;
            cout << ganttstr << endl ;
        } // setgantt


        void writefile( string inputstr ) {
            string outputfilename = "" ;
            outputfilename += inputstr ;
            outputfilename += "_method6.txt" ;
            ofstream outputfile ;
            outputfile.open( outputfilename ) ;
            if ( outputfile.is_open() ) {
                outputfile << "PPRR\n" ;
                outputfile << "==        PPRR==\n" ;
                outputfile << ganttstr + "\n" ;
                outputfile << "===========================================================\n\n" ;
                outputfile << "Waiting Time\n" ;
                outputfile << "ID\tPPRR\n" ;
                outputfile << "===========================================================\n" ;
                int index = 0 ;
                while ( index < finishlist.size() ) {
                    outputfile << finishlist[ index ].pid ;
                    outputfile << "\t" ;
                    outputfile << finishlist[ index ].waittime ;
                    outputfile << "\n" ;
                    index ++ ;
                } // while
                outputfile << "===========================================================\n\n" ;
                outputfile << "Turnaround Time\n" ;
                outputfile << "ID	PPRR\n" ;
                outputfile << "===========================================================\n" ;
                index = 0 ;
                while ( index < finishlist.size() ) {
                    outputfile << finishlist[ index ].pid ;
                    outputfile << "\t" ;
                    outputfile << finishlist[ index ].turnaroundtime ;
                    outputfile << "\n" ;
                    index ++ ;
                } // while
                outputfile << "===========================================================\n" ;
                cout << "write " << outputfilename << " successful " << endl ;
                outputfile.close() ;
            } // if
            else {
                cout << "error for write file " << endl ;
            } // else 
        } // writefile
} ; // PPRR

class ALL {
public :
    void writeallfile( string str, FCFS fcfs, RR rr, SJF sjf, SRTF srtf, HRRN hrrn, PPRR pprr ){
        string outputfilename = "" ;
        outputfilename += str ;
        outputfilename += "all" ;
        outputfilename += ".txt" ;
        ofstream outputfile ;
        outputfile.open( outputfilename ) ;
        if ( outputfile.is_open() ) {
            outputfile << "ALL\n" ;
            outputfile << "==   FCFS==\n" ;
            outputfile << fcfs.ganttstr << endl ;
            outputfile << "==   RR==\n" ;
            outputfile << rr.ganttstr << endl ;
            outputfile << "==   SJF==\n"  ;
            outputfile << sjf.ganttstr << endl ;
            outputfile << "==   SRTF==\n" ;
            outputfile << srtf.ganttstr << endl ;
            outputfile << "==   HRRN==\n" ;
            outputfile << hrrn.ganttstr << endl ;
            outputfile << "==   PPRR==\n" ;
            outputfile << pprr.ganttstr << endl ;
            outputfile << "===========================================================\n\n" ;
            outputfile << "Waiting Time\n" ;
            outputfile << "ID\tFCFS\tRR\tSJF\tSRTF\tHRRN\tPPRR\n" ;
            outputfile << "===========================================================\n" ;
            int index = 0 ;
            while( index < fcfs.finishlist.size() ) {
                outputfile << fcfs.finishlist[ index ].pid << "\t" ;
                outputfile << fcfs.finishlist[ index ].waittime << "\t" ;
                outputfile << rr.finishlist[ index ].waittime << "\t" ;
                outputfile << sjf.finishlist[ index ].waittime << "\t" ;
                outputfile << srtf.finishlist[ index ].waittime << "\t" ;
                outputfile << hrrn.finishlist[ index ].waittime << "\t" ;
                outputfile << pprr.finishlist[ index ].waittime << "\n" ;
                index ++ ; 
            } // while
            
            outputfile << "===========================================================\n\n" ;
            outputfile << "Turnaround Time\n" ;
            outputfile << "ID\tFCFS\tRR\tSJF\tSRTF\tHRRN\tPPRR\n" ;
            outputfile << "===========================================================\n" ;
            index = 0 ;
            while( index < fcfs.finishlist.size() ) {
                outputfile << fcfs.finishlist[ index ].pid << "\t" ;
                outputfile << fcfs.finishlist[ index ].turnaroundtime << "\t" ;
                outputfile << rr.finishlist[ index ].turnaroundtime << "\t" ;
                outputfile << sjf.finishlist[ index ].turnaroundtime << "\t" ;
                outputfile << srtf.finishlist[ index ].turnaroundtime << "\t" ;
                outputfile << hrrn.finishlist[ index ].turnaroundtime << "\t" ;
                outputfile << pprr.finishlist[ index ].turnaroundtime << "\n" ;
                index ++ ; 
            } // while
            outputfile << "===========================================================\n" ;
            cout << "wirte outputfile " << outputfilename << " successful " << endl ;
            outputfile.close() ;
        } // if
        else {
            cout << "output file write error " << outputfilename << endl ;
        } // esle
    } // writeallfile
} ; // ALL

int main() {
    //read input txt to list
    // create class to mission ( list )
    vector < Job > inputlist ;
    fstream inputfile ;
    string inputfilename = "" , inputstr = "" ; 
    string txt = ".txt" ;
    cout << "cin in filename input111.txt cin input111 : " << endl ;
    cin >> inputstr ;
    inputfilename += inputstr ;
    inputfilename += txt ;
    inputfile.open( inputfilename ) ;
    if ( inputfile.is_open() ) {
        int method = 0 ;
        inputfile >> method ;
        int timeslice = 0 ;
        inputfile >> timeslice ;
        // cout << method << timeslice << endl ; 
        string item = "" ;
        getline( inputfile , item ) ;
        item = "" ;
        getline( inputfile , item ) ;
        // cout << item << endl ;
        Job job ;
        int tempa = 0 , tempb = 0 ;
        inputfile >> tempa ;
        inputfile >> tempb ;
        while ( !inputfile.eof() ) {
            job.pid = tempa ;
            job.cpuburst = tempb ;
            inputfile >> job.arrival ;
            inputfile >> job.priority ;
            inputlist.push_back( job ) ;
            cout << job.pid << "\t  " << job.cpuburst << "\t  " << job.arrival << "\t\t" << job.priority << endl ;
            inputfile >> tempa ;
            inputfile >> tempb ;
        } // while

        // cout << inputlist.size() ;
        cout << "inputend" << endl ;
        inputfile.close() ;
        int mode = -1 ;
        cout << " 1 for FCFS" << endl ;
        cout << " 2 for RR" << endl ;
        cout << " 3 for SJF" << endl ;
        cout << " 4 for SRTF" << endl ;
        cout << " 5 for HRRN " <<  endl ;
        cout << " 6 for PPRR " << endl ;
        cout << " 7 for ALL " << endl ;
        cout << "cin mode : " << endl ;
        cin >> mode ;
        FCFS fcfs ;
        RR rr ;
        SJF sjf ;
        SRTF srtf ;
        HRRN hrrn ;
        PPRR pprr ;
        if ( mode == 1 || mode == 7 ) {
            fcfs.setJoblist( inputlist ) ;
            fcfs.schelduing() ;
            fcfs.setgantt() ;
            fcfs.sortfinishlist() ;
            fcfs.writefile( inputstr ) ;
         } // if
         if ( mode == 2 || mode == 7 ) {
            rr.setJoblist( inputlist ) ;
            rr.settimeslice( timeslice ) ;
            rr.schelduing() ;
            rr.setgantt() ;
            rr.sortfinishlist() ;
            rr.writefile( inputstr ) ;
         } // else if
         if ( mode == 3 || mode == 7 ) {
            sjf.setJoblist( inputlist ) ;
            sjf.schelduing() ;
            sjf.setgantt() ;
            sjf.sortfinishlist() ;
            sjf.writefile( inputstr ) ;
         } // else if
         if ( mode == 4 || mode == 7 ) {
            srtf.setJoblist( inputlist ) ;
            srtf.schelduing() ;
            srtf.setgantt() ;
            srtf.sortfinishlist() ;
            srtf.writefile( inputstr ) ;
         } // else if
         if ( mode == 5 || mode == 7 ) {
            hrrn.setJoblist( inputlist ) ;
            hrrn.schelduing() ;
            hrrn.setgantt() ;
            hrrn.sortfinishlist() ;
            hrrn.writefile( inputstr ) ;
         } //  else if
         if ( mode == 6 || mode == 7 ) {
            pprr.settimeslice( timeslice ) ;
            pprr.setJoblist( inputlist ) ;
            pprr.schelduing() ;
            pprr.setgantt() ;
            pprr.sortfinishlist() ;
            pprr.writefile( inputstr ) ;
         } // else if
         if ( mode == 7 ) {
            ALL all ;
            all.writeallfile( inputstr, fcfs, rr, sjf ,srtf , hrrn, pprr ) ;
         } // if ;

    } // if 
    else {
        cout << "error for open file " ;
    } // else

    // cout << "end " << endl ; 
} // main