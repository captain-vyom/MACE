/****************************************************************************

 Copyright (C) 2002-2014 Gilles Debunne. All rights reserved.

 This file is part of the QGLViewer library version 2.7.0.

 http://www.libqglviewer.com - contact@libqglviewer.com

 This file may be used under the terms of the GNU General Public License 
 versions 2.0 or 3.0 as published by the Free Software Foundation and
 appearing in the LICENSE file included in the packaging of this file.
 In addition, as a special exception, Gilles Debunne gives you certain 
 additional rights, described in the file GPL_EXCEPTION in this package.

 libQGLViewer uses dual licensing. Commercial/proprietary software must
 purchase a libQGLViewer Commercial License.

 This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

*****************************************************************************/

#include "../Viewer/board.h"
#include <fstream>
#include <stdio.h>

int main(int argc, char **argv) {
  Board board;

  if (argc != 3) {
    std::cout << "This AI blobwar player requires 2 parameters. It should be "
                 "called by the main blobWar viewer application."
              << std::endl;
    exit(1);
  }

  std::ifstream file(argv[1]);
  file >> board;
  file.close();

  std::cout << board.bestMoveNumberOfNewPieces();

  return 0;
}
