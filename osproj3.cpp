// 10920144
#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<regex>
#include<stack>
#include<iterator>
#include<queue>
using namespace std ;

class FIFO {
    public :
        FIFO() { maxframe = 0 ; }
        ~FIFO() {}
        vector<int> pageReference ;
        vector <int> pageframe ;
        int maxframe ;
        vector <string> record ;

        void setmaxframe( int num ) {
            maxframe = num ;
        } // setter

        void setseq( vector <int> inputseq ) {
            pageReference = inputseq ;
        } // setter

        bool isinpageframe( int pagenum ) {
            for ( int it = 0 ; it < pageframe.size() ; it ++ ) {
                if ( pageframe[ it ]  == pagenum ) {
                    return true ;
                } // if
            } // for
            return false ;
        } // isinpageframe

        void run() {
            bool pagefault = false ;
            string recordnow = "" ;
            string framestr = "" ;
            stringstream ss ;
            int pagefaultcount = 0 ;
            int pagereplacecount = 0 ;
            while ( pageReference.size() != 0 ) {
                int currentinpage = pageReference[ 0 ] ;
                pageReference.erase( pageReference.begin() ) ;
                // cout << "currentinpage : " << currentinpage << endl ;
                if ( isinpageframe( currentinpage ) == false ) {
                    if ( pageframe.size() < maxframe  ) {
                        pageframe.push_back( currentinpage ) ;
                        pagefault = true ;
                        pagefaultcount++ ;
                    }  // if
                    else {
                        pageframe.erase( pageframe.begin() ) ;
                        pageframe.push_back( currentinpage ) ;
                        pagefaultcount++ ;
                        pagereplacecount++ ;
                        pagefault = true ;
                    } // else

                } // if

                int i = pageframe.size() - 1 ;
                framestr = "" ;
                while ( i >= 0 ) {
                    int temp = pageframe[ i ] ;
                    ss << temp ;
                    framestr += ss.str() ;
                    ss.str(""); 
                    ss.clear() ;
                    i-- ;
                } // while
                recordnow = "" ;
                ss << currentinpage ;
                recordnow += ss.str() ;
                ss.str(""); 
                ss.clear() ;
                recordnow += "\t" ;
                recordnow += framestr ;
                recordnow += "\t" ;
                if ( pagefault ) {
                    recordnow += "F";
                    pagefault = false ;
                } // if              
                recordnow += "\n" ;
                record.push_back( recordnow ) ;
                recordnow = "" ;
            } // while


            string lastre = "Page Fault = " ;
            ss << pagefaultcount ;
            lastre += ss.str()  ;
            ss.str( "" ) ;
            ss.clear() ;
            lastre += "  Page Replaces = " ;
            ss << pagereplacecount ;
            lastre += ss.str()  ;
            ss.str( "" ) ;
            ss.clear() ;
            lastre += "  Page Frames = " ;
            ss << maxframe ;
            lastre += ss.str()  ;
            ss.str( "" ) ;
            ss.clear() ;
            lastre += "\n" ;  
            record.push_back( lastre ) ;
            int i = 0 ;
            while ( i < record.size() ) {
                cout << record[i] << endl ;
                i++ ;
            } // while
        } // run

        void writefile( string inputfilename  ) {
            ofstream outfile ;
            string outname = "output_" ;
            outname += inputfilename ;
            outname += "_method1" ;
            outname += ".txt" ;
            outfile.open( outname ) ;
            if ( outfile.is_open() ) {
                outfile << "--------------FIFO-----------------------\n" ;
                int index = 0 ;
                while ( index < record.size() ) {
                    outfile << record[ index ] ;
                    index++ ;
                } // while
                cout << "write outputfile successful : " << outname << endl ;
                outfile.close() ;
            } // if
            else {
                cout << "error for write file" << endl ;
            } // else 
        } // writefile
} ; // FIFO

struct LRUnode {
    int name ;
    int timerecord ;
} ; 

class LRU {
    public :
        LRU() { maxframe = 0 ; }
        ~LRU() {}
        vector<int> pageReference ;
        vector < LRUnode > pageframe ;
        int maxframe ;
        vector <string> record ;

        void setmaxframe( int num ) {
            maxframe = num ;
        } // setter

        void setseq( vector <int> inputseq ) {
            pageReference = inputseq ;
        } // setter

        bool isinpageframe( int pagenum, int currenttime ) {
            for ( int it = 0 ; it < pageframe.size() ; it ++ ) {
                if ( pageframe[ it ].name  == pagenum ) {
                    pageframe[ it ].timerecord = currenttime ;
                    return true ;
                } // if
            } // for
            return false ;
        } // isinpageframe

        void sorttimerecored() {
            LRUnode temp ;
            for ( int i = 0 ; i < pageframe.size() ; i++ ) {
                for ( int k = 0 ; k < pageframe.size() ; k++ ) {
                    if ( pageframe[ k ].timerecord > pageframe[ i ].timerecord ) {
                        temp = pageframe[ i ] ;
                        pageframe[ i ] = pageframe[ k ] ;
                        pageframe[ k ] = temp ;
                    } // if
                } // for
            } // for
        } // sorttimerecored

        void run() {
            bool pagefault = false ;
            string recordnow = "" ;
            string framestr = "" ;
            stringstream ss ;
            int pagefaultcount = 0 ;
            int pagereplacecount = 0, currenttime = 0 ;
            while ( pageReference.size() != 0 ) {
                int currentinpage = pageReference[ 0 ] ;
                pageReference.erase( pageReference.begin() ) ;
                // cout << "currentinpage : " << currentinpage << endl ;
                if ( isinpageframe( currentinpage , currenttime ) ) {
                    sorttimerecored() ;
                } // if
                else {
                  if ( pageframe.size() < maxframe  ) {
                        LRUnode node ;
                        node.name = currentinpage ;
                        node.timerecord = currenttime ;
                        pageframe.push_back( node ) ;
                        sorttimerecored() ;
                        pagefault = true ;
                        pagefaultcount++ ;
                    }  // if
                    else {
                        pageframe.erase( pageframe.begin() ) ;
                        LRUnode node ;
                        node.name = currentinpage ;
                        node.timerecord = currenttime ;
                        pageframe.push_back( node ) ;
                        sorttimerecored() ;
                        pagefaultcount++ ;
                        pagereplacecount++ ;
                        pagefault = true ;
                    } // else
                } // else

                int i = pageframe.size() - 1 ;
                framestr = "" ;
                while ( i >= 0 ) {
                    int temp = pageframe[ i ].name ;
                    ss << temp ;
                    framestr += ss.str() ;
                    ss.str(""); 
                    ss.clear() ;
                    i-- ;
                } // while
                // cout << "pageframe : " << endl ;
                // cout << framestr << endl ;

                recordnow = "" ;
                ss << currentinpage ;
                recordnow += ss.str() ;
                ss.str(""); 
                ss.clear() ;
                recordnow += "\t" ;
                recordnow += framestr ;
                recordnow += "\t" ;
                if ( pagefault ) {
                    recordnow += "F";
                    pagefault = false ;
                } // if              
                recordnow += "\n" ;
                record.push_back( recordnow ) ;
                recordnow = "" ;
                currenttime++ ;
            } // while


            string lastre = "Page Fault = " ;
            ss << pagefaultcount ;
            lastre += ss.str()  ;
            ss.str( "" ) ;
            ss.clear() ;
            lastre += "  Page Replaces = " ;
            ss << pagereplacecount ;
            lastre += ss.str()  ;
            ss.str( "" ) ;
            ss.clear() ;
            lastre += "  Page Frames = " ;
            ss << maxframe ;
            lastre += ss.str()  ;
            ss.str( "" ) ;
            ss.clear() ;
            lastre += "\n" ;  
            record.push_back( lastre ) ;
            int i = 0 ;
            while ( i < record.size() ) {
                cout << record[i] << endl ;
                i++ ;
            } // while
        } // run


        void writefile( string inputfilename  ) {
            ofstream outfile ;
            string outname = "output_" ;
            outname += inputfilename ;
            outname += "_method2" ;
            outname += ".txt" ;
            outfile.open( outname ) ;
            if ( outfile.is_open() ) {
                outfile << "--------------LRU-----------------------\n" ;
                int index = 0 ;
                while ( index < record.size() ) {
                    outfile << record[ index ] ;
                    index++ ;
                } // while
                cout << "write outputfile successful : " << outname << endl ;
                outfile.close() ;
            } // if
            else {
                cout << "error for write file" << endl ;
            } // else 
        } // writefile
} ; // LRU

struct LFUnode {
    int name ;
    int timerecord ;
    int count ;
} ; 

class LFU {
    public :
        LFU() { maxframe = 0 ; }
        ~LFU() {}
        vector<int> pageReference ;
        vector < LFUnode > pageframe ;
        int maxframe ;
        vector <string> record ;

        void setmaxframe( int num ) {
            maxframe = num ;
        } // setter

        void setseq( vector <int> inputseq ) {
            pageReference = inputseq ;
        } // setter

        bool isinpageframe( int pagenum ) {
            for ( int it = 0 ; it < pageframe.size() ; it ++ ) {
                if ( pageframe[ it ].name  == pagenum ) {
                    pageframe[ it ].count++ ;
                    return true ;
                } // if
            } // for
            return false ;
        } // isinpageframe

        void replacemincount( LFUnode inpage ) {
            int mintargetindex = 0 ;
            for ( int k = 0 ; k < pageframe.size() ; k++ ) {
                    if ( pageframe[ k ].count < pageframe[ mintargetindex ].count ) {
                        mintargetindex = k ;
                    } // if
                    else if ( pageframe[ k ].count == pageframe[ mintargetindex ].count ) {
                        if ( pageframe[ k ].timerecord < pageframe[ mintargetindex ].timerecord ) {
                            mintargetindex = k ;
                        } // if
                    } // else if 
                } // for
            pageframe.erase( pageframe.begin() + mintargetindex ) ;
            pageframe.push_back( inpage ) ;
        } // replacemincount

        void run() {
            bool pagefault = false ;
            string recordnow = "" ;
            string framestr = "" ;
            stringstream ss ;
            int pagefaultcount = 0 ;
            int pagereplacecount = 0, currenttime = 0 ;
            while ( pageReference.size() != 0 ) {
                int currentinpage = pageReference[ 0 ] ;
                pageReference.erase( pageReference.begin() ) ;
                // cout << "currentinpage : " << currentinpage << endl ;
                if ( isinpageframe( currentinpage ) ) {
                } // if
                else {
                  if ( pageframe.size() < maxframe  ) {
                        LFUnode node ;
                        node.count = 1 ;
                        node.name = currentinpage ;
                        node.timerecord = currenttime ;
                        pageframe.push_back( node ) ;
                        pagefault = true ;
                        pagefaultcount++ ;
                    }  // if
                    else {
                        LFUnode node ;
                        node.name = currentinpage ;
                        node.timerecord = currenttime ;
                        node.count = 1 ;
                        replacemincount( node ) ; // choose a min count ;
                        pagefaultcount++ ;
                        pagereplacecount++ ;
                        pagefault = true ;
                    } // else
                } // else

                int i = pageframe.size() - 1 ;
                framestr = "" ;
                while ( i >= 0 ) {
                    int temp = pageframe[ i ].name ;
                    ss << temp ;
                    framestr += ss.str() ;
                    ss.str(""); 
                    ss.clear() ;
                    i-- ;
                } // while
                // cout << "pageframe : " << endl ;
                // cout << framestr << endl ;

                recordnow = "" ;
                ss << currentinpage ;
                recordnow += ss.str() ;
                ss.str(""); 
                ss.clear() ;
                recordnow += "\t" ;
                recordnow += framestr ;
                recordnow += "\t" ;
                if ( pagefault ) {
                    recordnow += "F";
                    pagefault = false ;
                } // if              
                recordnow += "\n" ;
                record.push_back( recordnow ) ;
                recordnow = "" ;
                currenttime++ ;
            } // while


            string lastre = "Page Fault = " ;
            ss << pagefaultcount ;
            lastre += ss.str()  ;
            ss.str( "" ) ;
            ss.clear() ;
            lastre += "  Page Replaces = " ;
            ss << pagereplacecount ;
            lastre += ss.str()  ;
            ss.str( "" ) ;
            ss.clear() ;
            lastre += "  Page Frames = " ;
            ss << maxframe ;
            lastre += ss.str()  ;
            ss.str( "" ) ;
            ss.clear() ;
            lastre += "\n" ;  
            record.push_back( lastre ) ;
            int i = 0 ;
            while ( i < record.size() ) {
                cout << record[i] << endl ;
                i++ ;
            } // while
        } // run

        void writefile( string inputfilename  ) {
            ofstream outfile ;
            string outname = "output_" ;
            outname += inputfilename ;
            outname += "_method3" ;
            outname += ".txt" ;
            outfile.open( outname ) ;
            if ( outfile.is_open() ) {
                outfile << "--------------Least Frequently Used Page Replacement-----------------------\n" ;
                int index = 0 ;
                while ( index < record.size() ) {
                    outfile << record[ index ] ;
                    index++ ;
                } // while
                cout << "write outputfile successful : " << outname << endl ;
                outfile.close() ;
            } // if
            else {
                cout << "error for write file" << endl ;
            } // else 
        } // writefile
} ; // LFU

class MFU {
    public :
        MFU() { maxframe = 0 ; }
        ~MFU() {}
        vector<int> pageReference ;
        vector < LFUnode > pageframe ;
        int maxframe ;
        vector <string> record ;

        void setmaxframe( int num ) {
            maxframe = num ;
        } // setter

        void setseq( vector <int> inputseq ) {
            pageReference = inputseq ;
        } // setter

        bool isinpageframe( int pagenum ) {
            for ( int it = 0 ; it < pageframe.size() ; it ++ ) {
                if ( pageframe[ it ].name  == pagenum ) {
                    pageframe[ it ].count++ ;
                    return true ;
                } // if
            } // for
            return false ;
        } // isinpageframe


        void replacemincount( LFUnode inpage ) {
            int maxtargetindex = 0 ;
            for ( int k = 0 ; k < pageframe.size() ; k++ ) {
                    if ( pageframe[ k ].count > pageframe[ maxtargetindex ].count ) {
                        maxtargetindex = k ;
                    } // if
                    else if ( pageframe[ k ].count == pageframe[ maxtargetindex ].count ) {
                        if ( pageframe[ k ].timerecord < pageframe[ maxtargetindex ].timerecord ) {
                            maxtargetindex = k ;
                        } // if
                    } // else if 
                } // for

            pageframe.erase( pageframe.begin() + maxtargetindex ) ;
            pageframe.push_back( inpage ) ;

        } // replacemincount

        void run() {
            bool pagefault = false ;
            string recordnow = "" ;
            string framestr = "" ;
            stringstream ss ;
            int pagefaultcount = 0 ;
            int pagereplacecount = 0, currenttime = 0 ;
            while ( pageReference.size() != 0 ) {
                int currentinpage = pageReference[ 0 ] ;
                pageReference.erase( pageReference.begin() ) ;
                // cout << "currentinpage : " << currentinpage << endl ;
                if ( isinpageframe( currentinpage ) ) {
                } // if
                else {
                  if ( pageframe.size() < maxframe  ) {
                        LFUnode node ;
                        node.count = 1 ;
                        node.name = currentinpage ;
                        node.timerecord = currenttime ;
                        pageframe.push_back( node ) ;
                        pagefault = true ;
                        pagefaultcount++ ;
                    }  // if
                    else {
                        LFUnode node ;
                        node.name = currentinpage ;
                        node.timerecord = currenttime ;
                        node.count = 1 ;
                        replacemincount( node ) ; // choose a min count ;
                        pagefaultcount++ ;
                        pagereplacecount++ ;
                        pagefault = true ;
                    } // else
                } // else

                int i = pageframe.size() - 1 ;
                framestr = "" ;
                while ( i >= 0 ) {
                    int temp = pageframe[ i ].name ;
                    ss << temp ;
                    framestr += ss.str() ;
                    ss.str(""); 
                    ss.clear() ;
                    i-- ;
                } // while
                // cout << "pageframe : " << endl ;
                // cout << framestr << endl ;

                recordnow = "" ;
                ss << currentinpage ;
                recordnow += ss.str() ;
                ss.str(""); 
                ss.clear() ;
                recordnow += "\t" ;
                recordnow += framestr ;
                recordnow += "\t" ;
                if ( pagefault ) {
                    recordnow += "F";
                    pagefault = false ;
                } // if              
                recordnow += "\n" ;
                record.push_back( recordnow ) ;
                recordnow = "" ;
                currenttime++ ;
            } // while


            string lastre = "Page Fault = " ;
            ss << pagefaultcount ;
            lastre += ss.str()  ;
            ss.str( "" ) ;
            ss.clear() ;
            lastre += "  Page Replaces = " ;
            ss << pagereplacecount ;
            lastre += ss.str()  ;
            ss.str( "" ) ;
            ss.clear() ;
            lastre += "  Page Frames = " ;
            ss << maxframe ;
            lastre += ss.str()  ;
            ss.str( "" ) ;
            ss.clear() ;
            lastre += "\n" ;  
            record.push_back( lastre ) ;
            int i = 0 ;
            while ( i < record.size() ) {
                cout << record[i] << endl ;
                i++ ;
            } // while
        } // run

        void writefile( string inputfilename  ) {
            ofstream outfile ;
            string outname = "output_" ;
            outname += inputfilename ;
            outname += "_method4" ;
            outname += ".txt" ;
            outfile.open( outname ) ;
            if ( outfile.is_open() ) {
                outfile << "--------------Most Frequently Used Page Replacement -----------------------\n" ;
                int index = 0 ;
                while ( index < record.size() ) {
                    outfile << record[ index ] ;
                    index++ ;
                } // while
                cout << "write outputfile successful : " << outname << endl ;
                outfile.close() ;
            } // if
            else {
                cout << "error for write file" << endl ;
            } // else 
        } // writefile
} ; // MFU

class LFULRU {
    public :
        LFULRU() { maxframe = 0 ; }
        ~LFULRU() {}
        vector<int> pageReference ;
        vector < LFUnode > pageframe ;
        int maxframe ;
        vector <string> record ;

        void setmaxframe( int num ) {
            maxframe = num ;
        } // setter

        void setseq( vector <int> inputseq ) {
            pageReference = inputseq ;
        } // setter

        bool isinpageframe( int pagenum, int currenttime ) {
            for ( int it = 0 ; it < pageframe.size() ; it ++ ) {
                if ( pageframe[ it ].name  == pagenum ) {
                    pageframe[ it ].count++ ;
                    pageframe[ it ].timerecord = currenttime ;
                    return true ;
                } // if
            } // for

            return false ;

        } // isinpageframe
 
        void replacemincount( LFUnode inpage ) {
            int mintargetindex = 0 ;
            for ( int k = 0 ; k < pageframe.size() ; k++ ) {
                    if ( pageframe[ k ].count < pageframe[ mintargetindex ].count ) {
                        mintargetindex = k ;
                    } // if
                    else if ( pageframe[ k ].count == pageframe[ mintargetindex ].count ) {
                        if ( pageframe[ k ].timerecord < pageframe[ mintargetindex ].timerecord ) {
                            mintargetindex = k ;
                        } // if
                    } // else if 
                } // for

            pageframe.erase( pageframe.begin() + mintargetindex ) ;
            pageframe.push_back( inpage ) ;
        } // replacemincount

        void movetofirst( int inpagenum ) {
            int index = 0 ;
            for ( int i = 0 ; i < pageframe.size() ; i++ ) {
                if ( pageframe[ i ].name == inpagenum ) {
                    index = i ;
                    i = pageframe.size() ;
                } // if
            } // for

            LFUnode temp = pageframe[ index ] ;
            pageframe.erase( pageframe.begin() + index ) ; 
            pageframe.push_back( temp ) ;
        } // movetofirst

        void run() {
            bool pagefault = false ;
            string recordnow = "" ;
            string framestr = "" ;
            stringstream ss ;
            int pagefaultcount = 0 ;
            int pagereplacecount = 0, currenttime = 0 ;
            while ( pageReference.size() != 0 ) {
                int currentinpage = pageReference[ 0 ] ;
                pageReference.erase( pageReference.begin() ) ;
                // cout << "currentinpage : " << currentinpage << endl ;
                if ( isinpageframe( currentinpage, currenttime ) ) {
                    movetofirst( currentinpage ) ;
                } // if
                else {
                  if ( pageframe.size() < maxframe  ) {
                        LFUnode node ;
                        node.count = 1 ;
                        node.name = currentinpage ;
                        node.timerecord = currenttime ;
                        pageframe.push_back( node ) ;
                        pagefault = true ;
                        pagefaultcount++ ;
                    }  // if
                    else {
                        LFUnode node ;
                        node.name = currentinpage ;
                        node.timerecord = currenttime ;
                        node.count = 1 ;
                        replacemincount( node ) ; // choose a min count ;
                        pagefaultcount++ ;
                        pagereplacecount++ ;
                        pagefault = true ;
                    } // else
                } // else

                int i = pageframe.size() - 1 ;
                framestr = "" ;
                while ( i >= 0 ) {
                    int temp = pageframe[ i ].name ;
                    ss << temp ;
                    framestr += ss.str() ;
                    ss.str(""); 
                    ss.clear() ;
                    i-- ;
                } // while
                // cout << "pageframe : " << endl ;
                // cout << framestr << endl ;

                recordnow = "" ;
                ss << currentinpage ;
                recordnow += ss.str() ;
                ss.str(""); 
                ss.clear() ;
                recordnow += "\t" ;
                recordnow += framestr ;
                recordnow += "\t" ;
                if ( pagefault ) {
                    recordnow += "F";
                    pagefault = false ;
                } // if              
                recordnow += "\n" ;
                record.push_back( recordnow ) ;
                recordnow = "" ;
                currenttime++ ;
            } // while


            string lastre = "Page Fault = " ;
            ss << pagefaultcount ;
            lastre += ss.str()  ;
            ss.str( "" ) ;
            ss.clear() ;
            lastre += "  Page Replaces = " ;
            ss << pagereplacecount ;
            lastre += ss.str()  ;
            ss.str( "" ) ;
            ss.clear() ;
            lastre += "  Page Frames = " ;
            ss << maxframe ;
            lastre += ss.str()  ;
            ss.str( "" ) ;
            ss.clear() ;
            lastre += "\n" ;  
            record.push_back( lastre ) ;
            int i = 0 ;
            while ( i < record.size() ) {
                cout << record[i] << endl ;
                i++ ;
            } // while
        } // run

        void writefile( string inputfilename  ) {
            ofstream outfile ;
            string outname = "output_" ;
            outname += inputfilename ;
            outname += "_method5" ;
            outname += ".txt" ;
            outfile.open( outname ) ;
            if ( outfile.is_open() ) {
                outfile << "--------------Least Frequently Used LRU Page Replacement-----------------------\n" ;
                int index = 0 ;
                while ( index < record.size() ) {
                    outfile << record[ index ] ;
                    index++ ;
                } // while
                cout << "write outputfile successful : " << outname << endl ;
                outfile.close() ;
            } // if
            else {
                cout << "error for write file" << endl ;
            } // else 
        } // writefile
} ; // LFULRU

class ALL {
    public :
    ALL(){ }
    ~ALL() { }
    void writeallfile( FIFO fifo, LRU lru, LFU lfu, MFU mfu, LFULRU lfulru, string inputfilename ) {
        ofstream outfile ;
        string outname = "output_input" ;
        outname += inputfilename ;
        outname += ".txt" ;
        outfile.open( outname ) ;
        if ( outfile.is_open() ) {
            outfile << "--------------FIFO-----------------------\n" ;
            int index = 0 ;
            while ( index < fifo.record.size() ) {
                outfile << fifo.record[ index ] ;
                index++ ;
            } // while
            
            outfile << "--------------LRU-----------------------\n" ;
            index = 0 ;
            while ( index < lru.record.size() ) {
                outfile << lru.record[ index ] ;
                index++ ;
            } // while

            outfile << "--------------Least Frequently Used Page Replacement-----------------------\n" ;
            index = 0 ;
            while ( index < lfu.record.size() ) {
                outfile << lfu.record[ index ] ;
                index++ ;
            } // while

            index = 0 ;
            outfile << "--------------Most Frequently Used Page Replacement -----------------------\n" ;
            while ( index < mfu.record.size() ) {
                outfile << mfu.record[ index ] ;
                index++ ;
            } // while

            index = 0 ;
            outfile << "--------------Least Frequently Used LRU Page Replacement-----------------------\n" ;
            while ( index < lfulru.record.size() ) {
                outfile << lfulru.record[ index ] ;
                index++ ;
            } // while

            cout << "write all file successful : " << outname << endl ;
            outfile.close() ;
        } // if
        else {
            cout << "error for write file" << endl ;
        } // else 
        

    } // writeallfile
} ; // ALL

int main() {
    string inputfilenum = "", inputfilename = "" ;
    string strtxt = ".txt" ;
    cout << "cin filename ex : input555.txt cin input555 : " << endl ;
    cin >> inputfilenum ;
    inputfilename += inputfilenum ;
    inputfilename += strtxt ;
    fstream inputfile ;
    int framenum = 0 ;
    int model = 0 ;
    vector <int> inputseq ;
    inputfile.open( inputfilename ) ;
    if ( inputfile.is_open() ) {
        // cout << "su" << endl ;
        inputfile >> model ;
        // cout << " model :" << model << endl ;
        inputfile >> framenum ;
        // cout << " frame : " << framenum << endl ;
        char ch = '\0' ;
        inputfile >> ch ;
        while ( !inputfile.eof() ) {
            if ( isdigit( ch ) ) {
                int temp = ch - '0' ;
                inputseq.push_back( temp ) ;
            } // if
            inputfile >> ch ;
        } // while

        FIFO fifo ;
        LRU lru ;
        LFU lfu ;
        MFU mfu ;
        LFULRU lfulru ;
        if ( model == 1 || model == 6 ){
            fifo.setmaxframe( framenum ) ;
            fifo.setseq( inputseq  ) ;
            fifo.run() ;
            fifo.writefile( inputfilenum ) ;
        } // if
        if ( model == 2 || model == 6 ) {
            lru.setmaxframe( framenum ) ;
            lru.setseq( inputseq  ) ;
            lru.run() ;
            lru.writefile( inputfilenum ) ;
        } // if
        if ( model == 3 || model == 6 ) {
            lfu.setmaxframe( framenum ) ;
            lfu.setseq( inputseq ) ;
            lfu.run() ;
            lfu.writefile( inputfilenum ) ;
        } // if
        if ( model == 4 || model == 6 ) {
            mfu.setmaxframe( framenum ) ;
            mfu.setseq( inputseq ) ;
            mfu.run() ;
            mfu.writefile( inputfilenum ) ;
        } // if 
        if ( model == 5 || model == 6 ) {
            lfulru.setmaxframe( framenum ) ;
            lfulru.setseq( inputseq ) ;
            lfulru.run() ;
            lfulru.writefile( inputfilenum ) ;
        } // if
        if ( model == 6 ) {
            ALL all ;
            all.writeallfile( fifo, lru, lfu, mfu, lfulru, inputfilenum ) ;
        } // if

        inputfile.close() ;
    } // if
    else {
        cout << "error for open" << endl ;
    } // else
    
} // main