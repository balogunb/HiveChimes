#include "datadecoder.h"

DataDecoder::DataDecoder(DataContainer *container)
{

    dContainer = container;
    sa.sensors = 0;
    sa.sensorTime =0;
    sa.sensorBoard = 0;
    _type = false;
    miliseconds = 0;
}

/**
 * @brief The dcode method takes in the UDP address and splits it into seperate strings.
 * It then passes each of these strings into various methods.
 * @param _hex is the represenation of the UDP message
 */
void DataDecoder::decode(string _hex){
    d = new Data;
    cout << _hex << endl;
    vector<string> fragments; //vector containing each segment
    stringstream ss(_hex); // Turn the string into a stream.
    string tok;

    while(getline(ss, tok, '-')) {
        fragments.push_back(tok);
    }


    string _hiveId = fragments.at(0);
    string _date = dateDecoder(fragments.at(1));
    string _time = timeDecoder(fragments.at(1));
    d->hiveId = _hiveId;
    d->date = _date;
    d->time = _time;

    activityDecoder(fragments.at(2));






    //cout << "SIZE OF SENSOR SARRYA " << sensorArray.size() << endl;
   //dContainer.addData(d);
    sendExitAlert();
    sendEntryAlert();

    //dContainer.addData(d);



}

/**
 * @brief DataDecoder::dateDecoder The method reads in the UDP date and time and returns the date the event occured.
 * @param is the second part of the UDP message which is the date
 * @return string that represents the date of UDP message
 */
string DataDecoder:: dateDecoder(string s){
    stringstream ss(s); // Turn the string into a stream.
    string tok;

    getline(ss, tok, 'T'); //gets line up to character T
    tok.erase(tok.begin());  //erases D in the begining of string




    return tok;
}


/**
 * @brief DataDecoder::timeDecoder The method reads in the UDP date and time and returns the time the event occured.
 * The methos also sets the value of milliseconds for the UPD message, for comparison in future methods.
 * @param s is the second part of the UDP message
 * @return string that repreents the time in (hh.mm.ssmmm)
 */
string DataDecoder:: timeDecoder(string s){
    stringstream ss(s); // Turn the string into a stream.
    string tok;
    string time;
    vector <string> timeList;



    while(getline(ss, tok, 'T')) {
        getline(ss, tok, 'T');
        time = tok;


        unsigned long pos = time.find('.');
        string sub = time.substr(pos+3);
        cout << "sub" << sub<< endl;
        sub.erase(sub.begin());

        int milisec = std::stoi(sub);

        //set sensor time to
        sa.sensorTime = milisec;

    }
    long pos =long(time.find('.'));
    time.erase(time.begin()+pos);
    pos =long(time.find('.'));
    time.erase(time.begin()+pos);
    pos =long(time.find('.'));
    time.erase(time.begin()+pos);
    cout << "sensor time " <<sa.sensorTime << endl;
    return time;

}

/**
 * @brief DataDecoder::activityDecoder The method gets rid of the 'B's in the UDP message in order to decode the
 * acivity that occured. The method sets the boardNumber to the variable of struct sensorActivity.
 * @param _act is the third part of the UDP message
 */
void DataDecoder:: activityDecoder(string _act){
    stringstream act(_act);
    string delimiter = "B"; // set the delimeter
    vector<int> boardNum;

    int boardIndex=0;

    while(std::getline(act, delimiter, 'B'))
    {

        int board = std::atoi(delimiter.c_str()); //convert the delimiter to an int

        //get rid of any 0s for complexity purpose
        if(board != 0){

            boardNum.push_back(boardIndex); //this vector stores which board showed activity-- not sure if we need to know
            //make struct sensors equal to this board
            sa.sensorBoard = boardIndex;
            d->board = sa.sensorBoard;


            decimalToBinary(board); //pass the board activity to binary decoder


            cout << "board Activity decimal "  << board<< endl;
            cout << "board Index "  << boardIndex<< "\n"<< endl;


        }

        boardIndex ++; //increase index for each loop
    }



}

/**
 * @brief DataDecoder::compareSensors This method takes in a sensor of the struct sensorActivty and looks for the correcponding
 * sensor pair in the sensorArray. First the method checks the sensors time in miliseconds in the array. If the time is >500ms, delete it.
 * If the correspoding pair is found, then the sensor is deleted from the array. If it is not
 * found then the sensor is added to the array.
 * @param thisSensor
 */
void DataDecoder:: compareSensors(sensorActivity thisSensor){

    //if the array is empty just add sensor
    if(sensorArray.size() ==0){
        sensorArray.push_back(thisSensor);



        return;
    }

    //run through array
    for(unsigned i = 0; i<sensorArray.size(); i++){

        //remove all sensors that were > than 500s ago
        if(thisSensor.sensorTime - sensorArray.at(i).sensorTime > 500 ||
                sensorArray.at(i).sensorTime - thisSensor.sensorTime > 500)
        {
            // cout << "sensor 11" << thisSensor.sensors << endl;

            sensorArray.erase(sensorArray.begin() + (i));
            if(sensorArray.size() ==0 || i >= sensorArray.size()){
                break;
            }

        }


        if(thisSensor.sensorBoard == sensorArray.at(i).sensorBoard) //check only sensors of the same board
        {
            //  cout << "sensor22" << thisSensor.sensors << endl;
            if(sensorArray.at(i).sensors == getPair(thisSensor.sensors)){
                //send info into data base
                if(getPair(thisSensor.sensors) <=4){
                    cout << "PAIR WAS MADEEE" << endl;
                    d->type = 1;  //bee entered the hive
                    dContainer->addData(d);

                    //increase entry number
                    entryData++;

                }
                else if(getPair(thisSensor.sensors) > 4){
                    d->type = 0; // bee left the hive
                    cout << "PAIR WAS MADEEE" << endl;
                    dContainer->addData(d);

                    //increase exitData;
                    exitData++;
                }

                sensorArray.erase(sensorArray.begin() +(i)); //remove the struct in array
                return;
            }

        }

    }

    //if the sensor does not have a pair then add it to the array
    // cout << "sensor33" << thisSensor.sensors << endl;
    sensorArray.push_back(thisSensor);
    //cout << "sensor " << thisSensor.sensors << endl;

}
/**
 * @brief DataDecoder::compareSensors This method takes in a sensor of the struct sensorActivty and looks for the correcponding
 * sensor pair in the sensorArray. First the method checks the sensors time in miliseconds in the array. If the time is >500ms, delete it.
 * If the correspoding pair is found, then the sensor is deleted from the array. If it is not
 * found then the sensor is added to the array.
 * @param thisSensor
 */
void DataDecoder:: livestream(sensorActivity thisSensor){

    //if the array is empty just add sensor
    if(sensorArray.size() ==0){
        sensorArray.push_back(thisSensor);



        return;
    }

    //run through array
    for(unsigned i = 0; i<sensorArray.size(); i++){

        //remove all sensors that were > than 500s ago
        if(thisSensor.sensorTime - sensorArray.at(i).sensorTime > 500 ||
                sensorArray.at(i).sensorTime - thisSensor.sensorTime > 500)
        {
            // cout << "sensor 11" << thisSensor.sensors << endl;

            sensorArray.erase(sensorArray.begin() + (i));
            if(sensorArray.size() ==0 || i >= sensorArray.size()){
                break;
            }

        }


        if(thisSensor.sensorBoard == sensorArray.at(i).sensorBoard) //check only sensors of the same board
        {
            //  cout << "sensor22" << thisSensor.sensors << endl;
            if(sensorArray.at(i).sensors == getPair(thisSensor.sensors)){
                //send info into data base
                if(getPair(thisSensor.sensors) <=4){
                    cout << "PAIR WAS MADEEE" << endl;
                    d->type = 1;  //bee entered the hive
                    dContainer->loadData(d);

                    //increase entryData
                    entryData++;

                }
                else if(getPair(thisSensor.sensors) > 4){
                    d->type = 0; // bee left the hive
                    cout << "PAIR WAS MADEEE" << endl;
                    dContainer->loadData(d);

                    //increase exitData;
                    exitData++;
                }

                sensorArray.erase(sensorArray.begin() +(i)); //remove the struct in array
                return;
            }

        }

    }

    //if the sensor does not have a pair then add it to the array
    // cout << "sensor33" << thisSensor.sensors << endl;
    sensorArray.push_back(thisSensor);
    //cout << "sensor " << thisSensor.sensors << endl;

}

/**
 * @brief DataDecoder::decimalToBinary This method converts the decmial number input into a binary number
 * The binary number is then read backwards becuase arrayIndex's and binary indexs are reversed. If the index ==1
 * then set the senor to the index number.
 * @param boardAct
 */
void DataDecoder:: decimalToBinary(int boardAct){
    int _boardAct = boardAct;
    int binary[32];
    int count =0;

    //convert to binary
    while (_boardAct > 0){
        binary[count] = _boardAct %2;
        _boardAct = _boardAct/2;
        count++;


    }
    //binary is read in backwards
    for(int i = count-1; i >=0; i--){
        if(binary[i] == 1){
            sa.sensors = i+1; //set the sensor to the index == 1
            d->gate = sa.sensors;
            //  cout << "sensor " << sa.sensors << endl;
            livestream(sa);




        }
    }
}

/**
 * @brief DataDecoder::getPair This method takes int the sensor number and returns its corresponding pair.
 * @param sensorNum
 * @return the corresponding sensor pair
 */
int DataDecoder::getPair(int sensorNum){
    switch(sensorNum){
    case 1 : return 8;
    case 2 : return 7;
    case 3 : return 6;
    case 4 : return 5;
    case 5 : return 4;
    case 6 : return 3;
    case 7 : return 2;
    case 8 : return 1;

    default: return 0;


    }
}

bool DataDecoder::sendExitAlert(){
    int totalBees = exitData+ entryData;
    if(totalBees > 100){
        if(exitData > (totalBees/2)){
            return true;
        }else {
            return false;
        }
    }else{
        return false;

    }
}

bool DataDecoder::sendEntryAlert(){
    int totalBees = exitData+ entryData;
    if(totalBees > 100){
        if(entryData > (totalBees/2)){
            return true;
        }else {
            return false;
        }
    }else{
        return false;
    }
}








