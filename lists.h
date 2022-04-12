/*  Copyright 2008-2022 Carsten Elton Sorensen

    This file is part of ASMotor.

    ASMotor is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    ASMotor is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with ASMotor.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UTIL_LISTS_H_INCLUDED_
#define UTIL_LISTS_H_INCLUDED_

#include "types.h"

#define list_Data(type) \
    type* pPrev;        \
    type* pNext

#define list_Init(e)    do {(e)->pPrev=NULL;(e)->pNext=NULL;} while(0)
#define list_GetNext(e) ((e)->pNext)
#define list_GetPrev(e) ((e)->pPrev)
#define list_IsLast(e)  (list_GetNext(e)==NULL?true:false)
#define list_IsFirst(e) (list_GetPrev(e)==NULL?true:false)

#define list_Remove(head, e) do {        \
    if ((head) == (e)) {                 \
        (head) = list_GetNext(e);        \
        if (head)                        \
            list_GetPrev(head) = NULL;   \
    } else {                                                 \
        list_GetNext(list_GetPrev(e)) = list_GetNext(e);     \
        if((e) && !list_IsLast(e)) {                         \
            list_GetPrev(list_GetNext(e)) = list_GetPrev(e); \
        }                                                    \
    }                                                        \
} while(0)

#define list_Insert(head, e) do { \
    if (head)                     \
        list_GetPrev(head) = (e); \
    list_GetNext(e) = (head);     \
    list_GetPrev(e) = NULL;       \
    (head) = (e);                 \
} while(0)

#define list_InsertAfter(where, e) do {          \
    list_GetPrev(e) = (where);                   \
    list_GetNext(e) = list_GetNext(where);       \
    if (!list_IsLast(where))                     \
        list_GetPrev(list_GetNext(where)) = (e); \
    list_GetNext(where) = (e);                   \
} while(0)

#endif /* UTIL_LISTS_H_INCLUDED_ */
