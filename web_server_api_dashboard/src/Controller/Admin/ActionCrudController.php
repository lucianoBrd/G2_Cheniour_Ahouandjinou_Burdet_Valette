<?php

namespace App\Controller\Admin;

use App\Entity\Action;
use EasyCorp\Bundle\EasyAdminBundle\Field\TextField;
use EasyCorp\Bundle\EasyAdminBundle\Field\BooleanField;
use EasyCorp\Bundle\EasyAdminBundle\Field\AssociationField;
use EasyCorp\Bundle\EasyAdminBundle\Controller\AbstractCrudController;

class ActionCrudController extends AbstractCrudController
{
    public static function getEntityFqcn(): string
    {
        return Action::class;
    }

    
    public function configureFields(string $pageName): iterable
    {
        return [
            TextField::new('value'),
            BooleanField::new('state'),
            AssociationField::new('elements'),
        ];
    }
    
}
