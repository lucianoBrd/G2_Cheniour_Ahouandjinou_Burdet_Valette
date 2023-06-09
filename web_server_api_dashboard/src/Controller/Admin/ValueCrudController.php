<?php

namespace App\Controller\Admin;

use App\Entity\Value;
use EasyCorp\Bundle\EasyAdminBundle\Field\IdField;
use EasyCorp\Bundle\EasyAdminBundle\Field\TextField;
use EasyCorp\Bundle\EasyAdminBundle\Field\DateTimeField;
use EasyCorp\Bundle\EasyAdminBundle\Field\AssociationField;
use EasyCorp\Bundle\EasyAdminBundle\Controller\AbstractCrudController;

class ValueCrudController extends AbstractCrudController
{
    public static function getEntityFqcn(): string
    {
        return Value::class;
    }

    
    public function configureFields(string $pageName): iterable
    {
        return [
            IdField::new('id')->hideOnForm(),
            TextField::new('value'),
            DateTimeField::new('datetime')->hideOnForm(),
            AssociationField::new('element'),
        ];
    }
    
}
