#ifndef ZeroReverse_h
#define ZeroReverse_h
/**                                                                       
 * @file                                                                  
 * $Revision: 5059 $
 * $Date: 2013-03-11 09:37:00 -0700 (Mon, 11 Mar 2013) $
 * $Id: ZeroReverse.h 5059 2013-03-11 16:37:00Z slambright@GS.DOI.NET $
 * 
 *   Unless noted otherwise, the portions of Isis written by the USGS are 
 *   public domain. See individual third-party library and package descriptions 
 *   for intellectual property information, user agreements, and related  
 *   information.                                                         
 *                                                                        
 *   Although Isis has been used by the USGS, no warranty, expressed or   
 *   implied, is made by the USGS as to the accuracy and functioning of such 
 *   software and related material nor shall the fact of distribution     
 *   constitute any such warranty, and no responsibility is assumed by the
 *   USGS in connection therewith.                                        
 *                                                                        
 *   For additional information, launch                                   
 *   $ISISROOT/doc//documents/Disclaimers/Disclaimers.html                
 *   in a browser or see the Privacy &amp; Disclaimers page on the Isis website,
 *   http://isis.astrogeology.usgs.gov, and the USGS privacy and disclaimers on
 *   http://www.usgs.gov/privacy.html.                                    
 */                                                                       
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>


#include "IString.h"
#include "HiCalTypes.h"
#include "HiCalUtil.h"
#include "HiCalConf.h"
#include "Module.h"
#include "SplineFill.h"
#include "Statistics.h"
#include "SpecialPixel.h"
#include "IException.h"

namespace Isis {

  /**
   * @brief Processes Reverse Clock calibration data (ZeroReverse Module)
   * 
   * This class loads and processes the Reverse Clock data from a  HiRISE image
   * for offset correction purposes.   Additional processing may occur in 
   * subsequent modules. 
   *  
   * @ingroup Utility
   * 
   * @author 2008-06-13 Kris Becker 
   * @internal 
   *   @history 2010-04-16 Kris Becker Renamed from Zf to ZeroReverse
   *   @history 2010-10-28 Kris Becker Renamed parameters replacing "Zz" with
   *            "ZeroReverse".
   */
  class ZeroReverse : public Module {

    public: 
      //  Constructors and Destructor
      ZeroReverse() : Module("ZeroReverse") { }
      ZeroReverse(HiCalData &cal, const HiCalConf &conf) : 
                  Module("ZeroReverse") { 
        init(cal, conf);
      }

      /** Destructor */
      virtual ~ZeroReverse() { }

      /** 
       * @brief Return statistics for raw Reverse Clock buffer
       * 
       * @return const Statistics&  Statistics class with all stats
       */
      const Statistics &Stats() const { return (_stats); }

      /**
       * @brief Specifies if the input trigger conditions were met
       *  
       * If trigger conditions where met, the reverse clock correction becomes a 
       * constant as opposed to processed reverse clock pixels. 
       * 
       * @return bool True if triggered, false otherwise
       */
      bool wasTriggered() const { return (_triggered); }

    private:
      HiVector   _revClock;
      Statistics _stats;
      bool       _triggered;


      /**
       * @brief Initialize and compute data solution 
       *  
       * 
       * @author Kris Becker - 4/16/2010
       * 
       * @param cal  HiRISE calibration data provided 
       * @param conf Configuration data provider
       */
      void init(HiCalData &cal, const HiCalConf &conf) {
        DbProfile prof = conf.getMatrixProfile();
        _history.clear();
        _history.add("Profile["+ prof.Name()+"]");

        int line0 = toInt(ConfKey(prof, "ZeroReverseFirstLine", toString(0)));
        int lineN = toInt(ConfKey(prof, "ZeroReverseLastLine", toString(19)));
        QString tfile= conf.getMatrixSource("ReverseClockStatistics",prof);

        HiMatrix revclk = cropLines(cal.getReverseClock(), line0, lineN);
        _stats.Reset();
        _stats.AddData(revclk[0], revclk.dim1()*revclk.dim2());

        _revClock = averageLines(revclk);
       _history.add("RevClock(CropLines["+ToString(line0)+","+ToString(lineN) +
                    "],Mean["+ToString(_stats.Average()) + 
                     "],StdDev["+ToString(_stats.StandardDeviation()) +
                     "],LisPixels["+ToString(_stats.LisPixels())+
                     "],HisPixels["+ToString(_stats.HisPixels()) +
                     "],NulPixels["+ToString(_stats.NullPixels())+ "])");

       DbAccess triggers(Pvl(tfile).findObject("ReverseClockStatistics"));
       QString tprofName = conf.resolve("{FILTER}{CCD}_{CHANNEL}_{BIN}",prof); 
       _history.add("ReverseClockStatistics(File["+tfile+
                    "],Profile["+tprofName+"])");

       _triggered= false;
       if (triggers.profileExists(tprofName)) {
         DbProfile tprof(prof, triggers.getProfile(tprofName), tprofName);
         double revmean = toDouble(ConfKey(tprof,"RevMeanTrigger", toString(_stats.Average())));
         double revstddev = toDouble(ConfKey(tprof,"RevStdDevTrigger", toString(DBL_MAX)));
         int lisTol = toInt(ConfKey(tprof, "RevLisTolerance", toString(1)));
         int hisTol = toInt(ConfKey(tprof, "RevHisTolerance", toString(1)));
         int nulTol = toInt(ConfKey(tprof, "RevNulTolerance", toString(1)));

         _history.add("TriggerLimits(RevMeanTrigger["+ToString(revmean) +
                      "],RevStdDevTrigger["+ToString(revstddev)+
                      "],RevLisTolerance["+ToString(lisTol)+
                      "],RevHisTolerance["+ToString(hisTol)+
                      "],RevNulTolerance["+ToString(nulTol)+ "])");

         if ((_stats.LisPixels() > lisTol) || (_stats.HisPixels() > hisTol) ||
             (_stats.NullPixels() > nulTol) || 
             (_stats.StandardDeviation() > revstddev)) { 
           _triggered = true;
           _data = HiVector(_revClock.dim1(), revmean);
           _history.add("Trigger(True - Reverse Clock set to constant,"
                        "ReverseClock["+ToString(revmean)+"])"); 
         }
         else {
           _history.add("Trigger(False - Reverse Clock processing invoked)");
           _triggered = false;
         }
       }
       else {
         _history.add("Trigger(Profile["+tprofName+"],NotFound!)");
         _triggered = false;
       }

       if (!_triggered) {
        SplineFill spline(_revClock, _history);
        _data = spline.ref();
        _history = spline.History();
       }

        return;
      }

      /** Virtual dump of data processing vectors */
      virtual void printOn(std::ostream &o) const {
        o << "#  History = " << _history << std::endl;
        //  Write out the header
        o << std::setw(_fmtWidth)   << "RevClock"
          << std::setw(_fmtWidth+1) << "Applied\n";

        for (int i = 0 ; i < _data.dim() ; i++) {
          o << formatDbl(_revClock[i]) << " "
            << formatDbl(_data[i]) << std::endl;
        }
        return;
      }

  };

}     // namespace Isis
#endif

