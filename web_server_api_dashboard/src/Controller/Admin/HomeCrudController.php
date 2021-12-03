<?php

namespace App\Controller\Admin;

use App\Entity\Home;
use EasyCorp\Bundle\EasyAdminBundle\Field\TextField;
use EasyCorp\Bundle\EasyAdminBundle\Field\AssociationField;
use EasyCorp\Bundle\EasyAdminBundle\Controller\AbstractCrudController;

class HomeCrudController extends AbstractCrudController
{
    public static function getEntityFqcn(): string
    {
        return Home::class;
    }

    
    public function configureFields(string $pageName): iterable
    {
        return [
            TextField::new('label'),
            AssociationField::new('rooms'),
        ];
    }
    
}
