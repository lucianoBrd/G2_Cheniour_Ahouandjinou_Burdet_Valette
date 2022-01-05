<?php

namespace App\Controller\Admin;

use App\Entity\Mode;
use EasyCorp\Bundle\EasyAdminBundle\Field\IdField;
use EasyCorp\Bundle\EasyAdminBundle\Field\TextField;
use EasyCorp\Bundle\EasyAdminBundle\Field\BooleanField;
use EasyCorp\Bundle\EasyAdminBundle\Field\AssociationField;
use EasyCorp\Bundle\EasyAdminBundle\Controller\AbstractCrudController;

class ModeCrudController extends AbstractCrudController
{
    public static function getEntityFqcn(): string
    {
        return Mode::class;
    }

    
    public function configureFields(string $pageName): iterable
    {
        return [
            IdField::new('id')->hideOnForm(),
            TextField::new('label'),
            BooleanField::new('state'),
            AssociationField::new('home'),
        ];
    }
    
}
