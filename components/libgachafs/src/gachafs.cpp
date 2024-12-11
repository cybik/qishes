/******************************************************************
 *
 * gachafs.cpp
 *
 * \brief A brief summary of what this class intends to accomplish.
 *
 * Created by cybik on 24-06-29 for libgachafs, part of qgachawishes
 *
 ******************************************************************/
#include "gachafs.h"
#include <QDir>
#include <QFileInfo>

#include <string>
#include <filesystem>

#include <egachafs.h>

int gachafs::seek_depth(int level, const QStringList &stringList, const QFileInfo& fileInfo) {
    /** ** means "any bloody dir" so we don't go deeper */

    /** We matched with something not ** and not a file. */
    if(stringList.mid(level, 1).first().compare("**") != 0) return level + 1;

    /** Getting a hit on ** means we've matched "any" and the next, so skip 2 instead of 1 */
    if(stringList.mid(level+1, 1).first().compare(fileInfo.baseName()) == 0) return level + 2;

    /** Being on ** but not matching the next means we're still seeking under the filter of ** */
    return level;
}

std::shared_ptr<std::list<std::shared_ptr<QFile>>> gachafs::getFiles(
    const QString& filter, const QString& game_path, bool fail_ok
) {
    /**
     * Generalized(ish) file finder to get a given file following an ant-styled file filter descriptor.
     * This is unlikely to be truly functional. It's a bit of a hack.
     *
     * https://stackoverflow.com/a/27643657 adapted for deep directory traversal and ant filter approach
     **/
    QStringList matched_files = QStringList();
    std::function<void(const QString&, const QString&, int)> search_deep;

    // Define search_deep flow
    search_deep = [&search_deep, &matched_files](const QString& pattern, const QString& dirname, int level)
    {
        QDir dir = QDir(dirname);
        dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoSymLinks | QDir::NoDot | QDir::NoDotDot);
        static const QStringList stringList = pattern.split('/');
        for (const QFileInfo& fileInfo: dir.entryInfoList(
            stringList.count() == 1
                ? QStringList { stringList.last() }
                : stringList.mid(level, 1))
        ) {
            if (fileInfo.isDir() && fileInfo.isReadable()) {
                // Emulating ** behaviour
                search_deep( pattern, fileInfo.filePath(), seek_depth(level, stringList, fileInfo) );
            } else if (stringList.size() == (level + 1) ) {
                // We hit a file. Nice!
                matched_files.append(fileInfo.filePath());
            } else if (stringList.size() <= 2 && stringList.last().startsWith("*.")) {
                if (fileInfo.fileName().endsWith(stringList.last().last(stringList.last().size()-1))) {
                    matched_files.append(fileInfo.fileName()); // absolute f*cking trash wildcard support
                }
            }
        }
    };
    // Let's run.
    search_deep(filter, game_path, 0);
    std::shared_ptr<std::list<std::shared_ptr<QFile>>> stdlist = std::make_shared<std::list<std::shared_ptr<QFile>>>();
    if(!matched_files.empty()) {
        //std::cout << "Caches found" << std::endl;
        for(const auto& filepath: matched_files) {
            stdlist->push_back(std::make_shared<QFile>(filepath));
        }
    }
    if(stdlist->empty()) {
        if (!fail_ok) throw EGachaFS_Exception("No files found");
    }
    return stdlist;
}
