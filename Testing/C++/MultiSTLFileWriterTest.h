#ifndef MultiSTLFileWriterTest_h
#define MultiSTLFileWriterTest_h

#include <btkMultiSTLFileWriter.h>
#include <btkAcquisitionFileReader.h>
#include <btkTriangleMesh.h>

#include <fstream>

CXXTEST_SUITE(MultiSTLFileWriterTest)
{
  CXXTEST_TEST(MyCube)
  {
    btk::AcquisitionFileReader::Pointer reader = btk::AcquisitionFileReader::New();
    reader->SetFilename(C3DFilePathIN + "others/MyCube.c3d");
    
    std::vector<int> m(11);
    for (int i = 0 ; i < 11 ; ++i)
      m[i] = i;
    std::vector<btk::TriangleMesh::VertexLink> l(27);
    l[0].SetIds(0,3);
    l[1].SetIds(0,4);
    l[2].SetIds(0,5);
    l[3].SetIds(0,7);
    l[4].SetIds(0,8);
    l[5].SetIds(1,2);
    l[6].SetIds(1,5);
    l[7].SetIds(1,6);
    l[8].SetIds(1,8);
    l[9].SetIds(1,9);
    l[10].SetIds(2,6);
    l[11].SetIds(2,9);
    l[12].SetIds(3,6);
    l[13].SetIds(3,7);
    l[14].SetIds(3,8);
    l[15].SetIds(3,10);
    l[16].SetIds(4,5);
    l[17].SetIds(4,6);
    l[18].SetIds(4,7);
    l[19].SetIds(5,6);
    l[20].SetIds(5,8);
    l[21].SetIds(6,7);
    l[22].SetIds(6,9);
    l[23].SetIds(6,10);
    l[24].SetIds(8,9);
    l[25].SetIds(8,10);
    l[26].SetIds(9,10);
    btk::TriangleMesh::Pointer mesh = btk::TriangleMesh::New(m,l);
    
    btk::MultiSTLFileWriter::Pointer stlwriter = btk::MultiSTLFileWriter::New();
    stlwriter->SetInputAcquisition(reader->GetOutput());
    stlwriter->SetInputMesh(mesh);
    stlwriter->SetFilePrefix(STLFilePathOUT + "MyCube_");
    stlwriter->Update();
    
    std::string filename = STLFilePathOUT + "MyCube_1.stl";
    std::ifstream ifs(filename.c_str());
    TS_ASSERT(ifs.is_open());
    std::string header_ref = "STL binary file generated by BTK " + std::string(BTK_VERSION_STRING);
    char header[80] = {0};
    ifs.read(header,80);
    TS_ASSERT_EQUALS(header, header_ref);
    
    char nfc[4] = {0};
    ifs.read(nfc,4);
    int num_face = 0;
#if PROCESSOR_TYPE == 3 /* IEEE_BigEndian */
    num_face  = nfc[0] << 24 | nfc[1] << 16 | nfc[2] << 8 | nfc[3];
#elif PROCESSOR_TYPE == 2 /* VAX_LittleEndian */ 
    num_face  = nfc[1] << 24 | nfc[0] << 16 | nfc[3] << 8 | nfc[2];
#else
    num_face = *reinterpret_cast<int32_t const*>(nfc);
    int num_face2 = nfc[3] << 24 | nfc[2] << 16 | nfc[1] << 8 | nfc[0];
    TS_ASSERT_EQUALS(num_face, num_face2);
#endif
    TS_ASSERT_EQUALS(num_face, 19);
    // TS_WARN("Improve this test by with a binary file stream and check its content.");
    ifs.close();
    
    std::remove(filename.c_str());
  };
};

CXXTEST_SUITE_REGISTRATION(MultiSTLFileWriterTest)
CXXTEST_TEST_REGISTRATION(MultiSTLFileWriterTest, MyCube)

#endif