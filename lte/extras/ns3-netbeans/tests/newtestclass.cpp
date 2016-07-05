/*
 * File:   newtestclass.cpp
 * Author: makhtar
 *
 * Created on Feb 10, 2016, 4:35:46 PM
 */

#include "newtestclass.h"


CPPUNIT_TEST_SUITE_REGISTRATION (newtestclass);

newtestclass::newtestclass () { }

newtestclass::~newtestclass () { }

void
newtestclass::setUp () { }

void
newtestclass::tearDown () { }

void ns3::ProSeApp::StartLteD2d (std::list<uint64_t> lteMatches, bool noWifi);

void
newtestclass::testStartLteD2d ()
{
  std::list<uint64_t> lteMatches;
  bool noWifi;
  ns3::ProSeApp proSeApp;
  proSeApp.StartLteD2d (lteMatches, noWifi);
  if (true /*check result*/)
    {
      CPPUNIT_ASSERT (false);
    }
}

