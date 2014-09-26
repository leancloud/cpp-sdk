#define BOOST_TEST_MODULE AVFile 1.0 Test
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <boost/test/unit_test.hpp>
#include "AVOSCloud.h"
#include "AVFile/AVFile.h"

using namespace std;
using namespace avoscloud;

#error replace ${appId} and ${appKey} with real appId and appKey

BOOST_AUTO_TEST_CASE(AVFile_SaveFile_Test) {

  AVOSCloud::setApplicationId("${appId}", "${appKey}");
  AVFile* file = AVFile::fileWithPath("/Users/yangchaozhong/Downloads/avatar.jpeg");

  file->saveInBackgroundWithCallback([&](bool const& succeeded, AVError const& error){
      BOOST_CHECK(succeeded);
    });

  file->deleteInBackground();

  file->release();
}

BOOST_AUTO_TEST_CASE(AVFile_FetchFile_Test) {
  AVOSCloud::setApplicationId("${appId}", "${appKey}");
  AVFile* file = AVFile::fileWithUrl("http://ac-rucl8605.qiniudn.com/uuFoyM4Y2Ba6ObhC.jpeg");
  std::string path("/Users/yangchaozhong/avatar.jpeg");
  file->fetchFileDataIntoPath(path);

  std::ifstream ifs(path);
  if (ifs.good())
  {
    std::string content;
    ifs >> content;
    ifs.close();

    BOOST_CHECK(content.length() > 0);
  }

  BOOST_CHECK(remove(path.c_str()) == 0);

  file->release();
}
