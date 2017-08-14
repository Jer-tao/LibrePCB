/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 LibrePCB Developers, see AUTHORS.md for contributors.
 * http://librepcb.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*****************************************************************************************
 *  Includes
 ****************************************************************************************/
#include <QtCore>
#include "libraryelement.h"
#include <librepcb/common/fileio/domdocument.h>

/*****************************************************************************************
 *  Namespace
 ****************************************************************************************/
namespace librepcb {
namespace library {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

LibraryElement::LibraryElement(const QString& shortElementName,
                               const QString& longElementName, const Uuid& uuid,
                               const Version& version, const QString& author,
                               const QString& name_en_US, const QString& description_en_US,
                               const QString& keywords_en_US) :
    LibraryBaseElement(true, shortElementName, longElementName, uuid, version, author,
                       name_en_US, description_en_US, keywords_en_US)
{
}

LibraryElement::LibraryElement(const FilePath& elementDirectory, const QString& shortElementName,
                               const QString& longElementName, bool readOnly) :
    LibraryBaseElement(elementDirectory, true, shortElementName, longElementName, readOnly)
{
    // read category UUIDs
    DomElement& root = mLoadingXmlFileDocument->getRoot();
    foreach (const DomElement* node, root.getChilds("category")) {
        mCategories.append(node->getText<Uuid>(true));
    }
}

LibraryElement::~LibraryElement() noexcept
{
}

/*****************************************************************************************
 *  Protected Methods
 ****************************************************************************************/

void LibraryElement::serialize(DomElement& root) const
{
    LibraryBaseElement::serialize(root);
    foreach (const Uuid& uuid, mCategories) {
        root.appendTextChild("category", uuid);
    }
}

bool LibraryElement::checkAttributesValidity() const noexcept
{
    if (!LibraryBaseElement::checkAttributesValidity()) return false;
    foreach (const Uuid& uuid, mCategories) {
        if (uuid.isNull()) return false;
    }
    return true;
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace library
} // namespace librepcb