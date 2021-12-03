<?php

namespace App\Controller\Admin;

use App\Entity\Type;
use EasyCorp\Bundle\EasyAdminBundle\Field\IdField;
use EasyCorp\Bundle\EasyAdminBundle\Field\TextField;
use EasyCorp\Bundle\EasyAdminBundle\Field\AssociationField;
use EasyCorp\Bundle\EasyAdminBundle\Controller\AbstractCrudController;

class TypeCrudController extends AbstractCrudController
{
    public static function getEntityFqcn(): string
    {
        return Type::class;
    }

    
    public function configureFields(string $pageName): iterable
    {
        return [
            IdField::new('id')->hideOnForm(),
            TextField::new('label'),
            AssociationField::new('elements'),
        ];
    }
    
}
