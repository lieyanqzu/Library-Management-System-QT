#ifndef SETTING_H
#define SETTING_H

class Setting
{
public:
    static int getBorrowNumber();
    static int getRenewTimes();
    static double getFinePerDay();
    static void setBorrowNumber(int borrowNumber);
    static void setRenewTimes(int renewTimes);
    static void setFinePerDay(double finePerDay);
    static void initSetting();
    static void saveSetting();
private:
    static int borrow_number;
    static int renew_times;
    static double fine_per_day;
};

#endif // SETTING_H
