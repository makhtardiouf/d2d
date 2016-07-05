/*
 * File:   newtestclass.h
 * Author: makhtar
 *
 * Created on Feb 10, 2016, 4:35:45 PM
 */

#ifndef NEWTESTCLASS_H
#define	NEWTESTCLASS_H

#include <cppunit/extensions/HelperMacros.h>

class newtestclass : public CPPUNIT_NS::TestFixture {
    CPPUNIT_TEST_SUITE(newtestclass);

    CPPUNIT_TEST(testStartLteD2d);

    CPPUNIT_TEST_SUITE_END();

public:
    newtestclass();
    virtual ~newtestclass();
    void setUp();
    void tearDown();

private:
    void testStartLteD2d();

};

#endif	/* NEWTESTCLASS_H */

