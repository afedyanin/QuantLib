
/*
 Copyright (C) 2000, 2001, 2002 RiskMap srl

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
/*! \file linearinterpolation.hpp
    \brief linear interpolation between discrete points

    \fullpath
    ql/Math/%linearinterpolation.hpp
*/

// $Id$

#ifndef quantlib_linear_interpolation_h
#define quantlib_linear_interpolation_h

#include <ql/Math/interpolation.hpp>
#include <algorithm>

namespace QuantLib {

    namespace Math {

        //! linear interpolation between discrete points
        template <class RandomAccessIterator1, class RandomAccessIterator2>
        class LinearInterpolation
        : public Interpolation<RandomAccessIterator1,RandomAccessIterator2> {
          public:
            /*  these typedefs are repeated because Borland C++ won't inherit
                them from Interpolation - they shouldn't hurt, though.
            */
            typedef
              typename QL_ITERATOR_TRAITS<RandomAccessIterator1>::value_type
                argument_type;
            typedef
              typename QL_ITERATOR_TRAITS<RandomAccessIterator2>::value_type
                result_type;
             LinearInterpolation(const RandomAccessIterator1& xBegin,
                 const RandomAccessIterator1& xEnd,
                 const RandomAccessIterator2& yBegin,
                 bool allowExtrapolation)
             : Interpolation<RandomAccessIterator1,RandomAccessIterator2>(
                 xBegin,xEnd,yBegin, allowExtrapolation) {}
            result_type operator()(const argument_type& x) const;
        };


        // inline definitions

        template <class I1, class I2>
        inline LinearInterpolation<I1,I2>::result_type
        LinearInterpolation<I1,I2>::operator()(
            const LinearInterpolation<I1,I2>::argument_type& x) const {
                I1 i;
                if (x < *xBegin_) {
                    QL_REQUIRE(allowExtrapolation_,
                        "LinearInterpolation::operator() : "
                        "extrapolation not allowed "
                        "[x<xMin]");
                    i = xBegin_;
                } else if (x > *(xEnd_-1)) {
                    QL_REQUIRE(allowExtrapolation_,
                        "LinearInterpolation::operator() : "
                        "extrapolation not allowed "
                        "[x>xMax]");
                    i = xEnd_-2;
                } else
                    i = std::upper_bound(xBegin_,xEnd_-1,x)-1;
                I2 j = yBegin_+(i-xBegin_);
                return *j + (x-*i)*double(*(j+1)-*j)/double(*(i+1)-*i);
        }

    }

}


#endif
