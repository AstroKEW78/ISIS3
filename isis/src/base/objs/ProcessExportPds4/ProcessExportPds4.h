#ifndef ProcessExportPds4_h
#define ProcessExportPds4_h
/*
 *   Unless noted otherwise, the portions of Isis written by the
 *   USGS are public domain. See individual third-party library
 *   and package descriptions for intellectual property
 *   information,user agreements, and related information.
 *
 *   Although Isis has been used by the USGS, no warranty, expressed or implied,
 *   is made by the USGS as to the accuracy and functioning of such software
 *   and related material nor shall the fact of distribution constitute any such
 *   warranty, and no responsibility is assumed by the USGS in connection
 *   therewith.
 *
 *   For additional information, launch
 *   $ISISROOT/doc//documents/Disclaimers/Disclaimers.html in a browser or see
 *   the Privacy &amp; Disclaimers page on the Isis website,
 *   http://isis.astrogeology.usgs.gov, and the USGS privacy and disclaimers on
 *   http://www.usgs.gov/privacy.html.
 */

#include "ProcessExport.h"
#include <vector>
#include <QString>
#include <QDomDocument>

namespace Isis {

  /**
   * @brief Process class for exporting cubes to PDS4 standards
   *
   * This class extends the ProcessExport class to allow the user
   * to export cubes to PDS4 format.
   *
   * @author 2017-05-25 Marjorie Hahn and Makayla Shepherd
   * @internal
   *   @history 2017-05-31 Ian Humphrey - Added check in StandardPds4Label to thrown an
   *                           exception if there is no input cube set.
   *   @history 2017-06-01 Ian Humphrey - Added XML declaration and updated attributes for
   *                           Product_Observational tag.
   *   @history 2017-06-02 Marjorie Hahn - Added global hash seed to force a consistent
   *                           output (XML attribute order).
   *   @history 2017-06-02 Adam Paquette - Updated call to translation file to use a translation file
   *                           in the isis data area.
   *   @history 2017-06-02 Makayla Shepherd - Added CreateImageLabel, FixedImageRoot, and
   *                           StandardImageImage to add hardcoded values to the xml label.
   *   @history 2017-06-04 Adam Paquette - Added GetLabel function and updated StandardPds4Label.
   *   @history 2017-06-08 Marjorie Hahn - Added WritePds4 method to write out the 
   *                           .img and .xml Pds4 data.
   *   @history 2017-09-26 Jesse Mapel - Improved test coverage and documentation. Fixes #5167.
   *   @history 2017-10-18 Jeannie Backer & Makayla Shepherd - Added convenience method getElement
   *                           and StandardAllMapping method to translate mapping information.
   *                           See #5202.
   *   @history 2017-10-31 Jeannie Backer - Added standardInstrument() and displaySettings()
   *                           translations.
   *   @history 2017-11-06 Kristin Berry - Added standardBandBin() 
   *   @history 2017-11-07 Jeannie Backer - Added code to search for target in the Mapping group
   *                           if no instrument group is found. Added Identification Area
   *                           tranlations. Added sanity checks to getElement().
   *   @history 2017-11-07 Jeannie Backer - Added code to write data file info into label file.
   *                           Added code to translate time format and add nil tag if empty.
   *   @history 2017-11-15 Jesse Mapel - Added translateUnits method to convert units to PDS4
   *                           standard format.
   *   @history 2017-11-16 Kristin Berry - Updated WritePds4 to put the File information first
   *                           in the output File_Area_Observational.
   *   @history 2017-11-17 Jesse Mapel - Removed empty FixedImageRoot method.
   *   @history 2017-11-20 Jeannie Backer - Updated StandardImageImage() to re-order the
   *                           Array_3D_Image values properly.
   *   @history 2017-11-21 Kristin Berry - Updated the constructor to add the xml version and 
   *                           encoding to the beginning of the XML file.
   *   @history 2018-02-05 Kristin Berry - Updated WritePds4 to remove the .xml and add a .img
   *                           if the user inputs something of the form filename.xml as the image
   *                           output name. 
   */

  class ProcessExportPds4: public Isis::ProcessExport {
    public:

      ProcessExportPds4();
      ~ProcessExportPds4();

      QDomDocument &StandardPds4Label();
      void StandardAllMapping();

      void CreateImageLabel();
      void StandardImageImage();

      void OutputLabel(std::ofstream &os);

      // Include this using declaration to indicate that ProcessExportPds4
      // objects that call a StartProcess() method that has not been overridden
      // here should use the corresponding base class definitions
      using ProcessExport::StartProcess;
      void StartProcess(std::ofstream &fout);
      QDomDocument &GetLabel();
      void WritePds4(QString outFile);
      QDomElement getElement(QStringList xmlPath, QDomElement parent=QDomElement());
      void addHistory(QString description, QString date = "tbd", QString version = "1.0");

      static void translateUnits(QDomDocument &label,
                                 QString transMapFile = "$base/translations/pds4ExportUnits.pvl");

    protected:
      void addSchema(QString sch, QString xsd, QString xmlns, QString xmlnsURI) ;
      void identificationArea();
      void standardInstrument();
      void standardBandBin(); 
      void displaySettings();
      QString PDS4PixelType(PixelType pixelType, ByteOrder endianType);
      static QMap<QString, QString> createUnitMap(Pvl configPvl);
      static void translateChildUnits(QDomElement parent, QMap<QString, QString> transMap);

      QDomDocument *m_domDoc;               //!< XML label
      QString m_schemaLocation;             //!< QString with all schema locations required

  };
}

#endif
