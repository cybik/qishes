//
// Created by cybik on 24-06-29.
//

#include "gachafs.h"
#include <QDir>
#include <QFileInfo>

std::shared_ptr<std::list<std::shared_ptr<QFile>>> gachafs::getFiles(const QString& filter, QString& command_game_path) {
    /**
     * Generalized(ish) file finder to get a given file following an ant-styled file filter descriptor.
     * This is unlikely to be truly functional. It's a bit of a hack.
     *
     * https://stackoverflow.com/a/27643657 adapted for deep directory traversal and ant filter approach
     **/
    std::function<void(const QString&, const QString&, int)> search_deep;
    QStringList matched_files = QStringList();
    search_deep = [filter, &search_deep, &matched_files](const QString& pattern, const QString& dirname, int level)
    {
        QDir dir = QDir(dirname);
        dir.setFilter(
            QDir::Dirs | QDir::Files | QDir::NoSymLinks | QDir::NoDot | QDir::NoDotDot
        );
        static const QStringList stringList = filter.split('/');
            foreach (QFileInfo fileInfo, dir.entryInfoList(stringList.mid(level, 1))) {
            if (fileInfo.isDir() && fileInfo.isReadable()) {
                // Emulating ** behaviour
                if(stringList.mid(level, 1).first().compare("**") == 0) {
                    if(stringList.mid(level+1, 1).first().compare(fileInfo.baseName()) != 0) {
                        // ** means "any bloody dir" so we don't go deeper into the pattern
                        search_deep( pattern, fileInfo.filePath(), level );
                    } else {
                        // Getting a hit on ** means we've matched "any" and the next, so skip 2 instead of 1
                        search_deep( pattern, fileInfo.filePath(), level + 2 );
                    }
                } else {
                    // We matched with something not ** and not a file.
                    search_deep( pattern, fileInfo.filePath(), level + 1 );
                }
            } else if (stringList.size() == (level + 1) ) {
                // We hit a file. Nice!
                matched_files.append(fileInfo.filePath());
            }
        }
    };
    search_deep(filter, command_game_path, 0);
    std::shared_ptr<std::list<std::shared_ptr<QFile>>> stdlist = std::make_shared<std::list<std::shared_ptr<QFile>>>();
    if(!matched_files.empty()) {
        //std::cout << "Caches found" << std::endl;
        foreach(QString filepath, matched_files) {
            //std::cout << filepath.toStdString() << std::endl;
            auto f =    std::make_unique<QFile>(filepath);
            stdlist->push_back(std::move(f));
        }
    }
    return stdlist;
}

gachafs::gachafs() {

}
gachafs::~gachafs() {

}
