
/*
 Copyright (C) 2000, 2001, 2002, 2003 RiskMap srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it under the
 terms of the QuantLib license.  You should have received a copy of the
 license along with this program; if not, please email ferdinando@ametrano.net
 The license is also available online at http://quantlib.org/html/license.html

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file calendar.cpp
    \brief Abstract calendar class
*/

#include <ql/calendar.hpp>

namespace QuantLib {

    Date Calendar::roll(const Date& d ,
			RollingConvention c,
			const Date& origin) const {
        QL_REQUIRE(d!=Date(), "Calendar::roll : null date");
        Date d1 = d;
        if (c == Following || c == ModifiedFollowing ||
	    c == MonthEndReference) {
            while (isHoliday(d1))
                d1++;
            if (c == ModifiedFollowing || c == MonthEndReference) {
	       if (d1.month() != d.month()) {
		  return roll(d,Preceding);
	       }
	       if (c == MonthEndReference && origin != Date()) {
		  if (isLastBusinessDayOfMonth(origin) &&
		      !isLastBusinessDayOfMonth(d1)) {
		     d1 = Date(d1.lastDayOfMonth(),d1.month(),d1.year());
		     return roll(d1,Preceding);
		  }
	       }
	    }
        } else if (c == Preceding || c == ModifiedPreceding) {
	    while (isHoliday(d1))
	       d1--;
	    if (c == ModifiedPreceding && d1.month() != d.month()) {
	       return roll(d,Following);
	    }
        } else {
	    throw Error("Unknown rolling convention");
        }
	return d1;
    }

    Date Calendar::advance(const Date& d, int n, TimeUnit unit,
      RollingConvention c) const {
        QL_REQUIRE(d!=Date(), "Calendar::roll : null date");
        if (n == 0) {
            return roll(d,c);
        } else if (unit == Days) {
            Date d1 = d;
            if (n > 0) {
                while (n > 0) {
                    d1++;
                    while (isHoliday(d1))
                        d1++;
                    n--;
                }
            } else {
                while (n < 0) {
                    d1--;
                    while(isHoliday(d1))
                        d1--;
                    n++;
                }
            }
            return d1;
        } else {
            Date d1 = d.plus(n,unit);
            return roll(d1,c,d);
        }
        QL_DUMMY_RETURN(Date());
    }

    Date Calendar::advance(const Date & d,
                           const Period & p,
                           RollingConvention c)const {
        return advance(d,
                       p.length(),
                       p.units(),
                       c);
    }

   // Western calendars

    Day Calendar::WesternImpl::easterMonday(Year y) {
        static const Day EasterMonday[] = {
            107,  98,  90, 103,  95, 114, 106,  91, 111, 102,   // 1900-1909
             87, 107,  99,  83, 103,  95, 115,  99,  91, 111,   // 1910-1919
             96,  87, 107,  92, 112, 103,  95, 108, 100,  91,   // 1920-1929
            111,  96,  88, 107,  92, 112, 104,  88, 108, 100,   // 1930-1939
             85, 104,  96, 116, 101,  92, 112,  97,  89, 108,   // 1940-1949
            100,  85, 105,  96, 109, 101,  93, 112,  97,  89,   // 1950-1959
            109,  93, 113, 105,  90, 109, 101,  86, 106,  97,   // 1960-1969
             89, 102,  94, 113, 105,  90, 110, 101,  86, 106,   // 1970-1979
             98, 110, 102,  94, 114,  98,  90, 110,  95,  86,   // 1980-1989
            106,  91, 111, 102,  94, 107,  99,  90, 103,  95,   // 1990-1999
            115, 106,  91, 111, 103,  87, 107,  99,  84, 103,   // 2000-2009
             95, 115, 100,  91, 111,  96,  88, 107,  92, 112,   // 2010-2019
            104,  95, 108, 100,  92, 111,  96,  88, 108,  92,   // 2020-2029
            112, 104,  89, 108, 100,  85, 105,  96, 116, 101,   // 2030-2039
             93, 112,  97,  89, 109, 100,  85, 105,  97, 109,   // 2040-2049
            101,  93, 113,  97,  89, 109,  94, 113, 105,  90,   // 2050-2059
            110, 101,  86, 106,  98,  89, 102,  94, 114, 105,   // 2060-2069
             90, 110, 102,  86, 106,  98, 111, 102,  94, 107,   // 2070-2079
             99,  90, 110,  95,  87, 106,  91, 111, 103,  94,   // 2080-2089
            107,  99,  91, 103,  95, 115, 107,  91, 111, 103    // 2090-2099
        };
        return EasterMonday[y-1900];
    }

}
